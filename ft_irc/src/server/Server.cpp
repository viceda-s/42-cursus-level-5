#include "Server.hpp"
#include "IrcReplies.hpp"
#include "FileTransfer.hpp"
#include <asm-generic/socket.h>
#include <cstddef>
#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

//
/* Public */
//

Server::Server(int port, const std::string& password) :
	_port(port), _password(password), _server_fd(-1) {
	_setupSocket();
}

Server::~Server() {
	// Close all client connections
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		close(it->first);
	}
	
	// Delete all channels
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		delete it->second;
	}
	
	// Close server socket
	if (_server_fd != -1)
		close(_server_fd);
}

void Server::run() {
	std::cout << "Server running on port " << _port << ". Waiting for connections..." << std::endl;

	while (true) {
		// Register any DCC transfer FDs before polling
		_registerTransferFds();

		int poll_count = poll(_poll_fds.data(), _poll_fds.size(), 100); // 100ms timeout for DCC
		if (poll_count < 0) {
			if (errno == EINTR) continue;
			throw std::runtime_error("Poll failed");
		}

		for (size_t i = 0; i < _poll_fds.size(); i++) {
			if (_poll_fds[i].revents == 0) {
				continue;
			}

			// Check for errors
			if (_poll_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
				if (_poll_fds[i].fd == _server_fd) {
					continue;
				}
				// Check if it's a transfer FD
				bool is_transfer = false;
				for (std::map<std::string, FileTransfer*>::iterator it = _active_transfers.begin();
					 it != _active_transfers.end(); ++it) {
					if (it->second->getListenFd() == _poll_fds[i].fd ||
						it->second->getTransferFd() == _poll_fds[i].fd) {
						is_transfer = true;
						it->second->abort();
						break;
					}
				}
				if (!is_transfer) {
					_removeClient(_poll_fds[i].fd);
					i--;
				}
				continue;
			}

			// Handle POLLIN
			if (_poll_fds[i].revents & POLLIN) {
				if (_poll_fds[i].fd == _server_fd) {
					_acceptNewClient();
				} else {
					// Check if it's a DCC listen FD
					bool handled = false;
					for (std::map<std::string, FileTransfer*>::iterator it = _active_transfers.begin();
						 it != _active_transfers.end(); ++it) {
						if (it->second->getListenFd() == _poll_fds[i].fd) {
							it->second->acceptConnection();
							handled = true;
							break;
						}
					}
					if (!handled) {
						_handleClientData(_poll_fds[i].fd);
					}
				}
			}

			// Handle POLLOUT for client output buffers
			if (_poll_fds[i].revents & POLLOUT) {
				// Check if it's a client with pending output
				if (_output_buffers.find(_poll_fds[i].fd) != _output_buffers.end() &&
					!_output_buffers[_poll_fds[i].fd].empty()) {
					_flushClientBuffer(_poll_fds[i].fd);
				}
				// Check if it's a DCC transfer FD
				for (std::map<std::string, FileTransfer*>::iterator it = _active_transfers.begin();
					 it != _active_transfers.end(); ++it) {
					if (it->second->getTransferFd() == _poll_fds[i].fd) {
						if (!it->second->sendFileData()) {
							if (it->second->isComplete()) {
								std::cout << "\n✓ File transfer complete: " << it->first << std::endl;
							} else {
								std::cout << "\n✗ File transfer failed: " << it->first << std::endl;
							}
						}
						break;
					}
				}
			}
		}

		_cleanupCompletedTransfers();
	}
}

//
/* Socket */
//

void Server::_setupSocket() {
	// Create server SOCKET
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0) {
		throw std::runtime_error("Failed to create socket");
	}

	// Allow port reuse
	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(_server_fd);
		throw std::runtime_error("Failed to set socket options");
	}

	// Bind to port
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	if (bind(_server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		close(_server_fd);
		std::ostringstream oss;
		oss << "Failed to bind socket to port " << _port;
		throw std::runtime_error(oss.str());
	}

	// Listen to connections
	if (listen(_server_fd, LISTEN_CONN) < 0) {
		close(_server_fd);
		throw std::runtime_error("Failed to listen on server socket");
	}

	_setNonBlocking(_server_fd);

	// Add server socket to poll array
	struct pollfd server_pollfd;
	server_pollfd.fd = _server_fd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	_poll_fds.push_back(server_pollfd);
}

void Server::_acceptNewClient() {
	// Accept connection
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	int client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0) {
		std::cerr << "Failed to accept client connection" << std::endl;
		return;
	}

	// Set non blocking
	_setNonBlocking(client_fd);

	// Add to poll array
	struct pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	_poll_fds.push_back(client_pollfd);

	// initialize CLient instance and add it to client array
	_clients[client_fd] = Client(client_fd);
	std::cout << "New client connected: fd=" << client_fd << std::endl;
}

void Server::_handleClientData(int client_fd) {
	// read client data to buffer
	char buffer[512];
	int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_read <= 0) {
		if (bytes_read == 0) {
			std::cout << "Client disconnected: fd=" << client_fd << std::endl;
		} else {
			std::cerr << "Error reading from client: fd=" << client_fd << std::endl;
		}
		_removeClient(client_fd);
		return;
	}

	// Append read buffer to client buffer
	buffer[bytes_read] = '\0';
	_clients[client_fd].addToBuffer(std::string(buffer, bytes_read));
	// Process comand
	_processClientCommands(client_fd);
}

void Server::_setNonBlocking(int fd) {
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

//
/* Command processing */
//

void Server::_processClientCommands(int client_fd) {
	// Check if message is complete (\r\n)
	while (true) {
		// Check if client still exists (might have been removed by previous command)
		if (_clients.find(client_fd) == _clients.end()) {
			return;
		}
		
		std::string& buffer = _clients[client_fd].getBuffer();
		size_t pos = buffer.find("\r\n");
		
		if (pos == std::string::npos) {
			break;
		}
		
		// Get command from buffer (not including \r\n)
		std::string command = buffer.substr(0, pos);
		// Clear command from buffer
		buffer.erase(0, pos + 2);

		// If not empty, execute
		if (!command.empty()) {
			std::cout << "Client " << client_fd << ": " << command << std::endl;
			_executeCommand(client_fd, command);
		}
	}

	// Also handle \n only (some clients might not send \r\n)
	while (true) {
		// Check if client still exists
		if (_clients.find(client_fd) == _clients.end()) {
			return;
		}
		
		std::string& buffer = _clients[client_fd].getBuffer();
		size_t pos = buffer.find("\n");
		
		if (pos == std::string::npos) {
			break;
		}
		
		std::string command = buffer.substr(0, pos);
		buffer.erase(0, pos + 1);

		if (!command.empty() && command[command.length() - 1] == '\r')
			command.erase(command.length() - 1);

		if (!command.empty()) {
			std::cout << "Client " << client_fd << ": " << command << std::endl;
			_executeCommand(client_fd, command);
		}
	}
}

void Server::_executeCommand(int client_fd, const std::string& command) {
	// Tokenize command
	std::vector<std::string> tokens = _splitCommand(command);
	if (tokens.empty())
		return;

	std::string cmd = tokens[0];
	std::vector<std::string> params(tokens.begin() + 1, tokens.end());

	// Convert command to uppercase
	for (size_t i = 0; i < cmd.length(); i++)
		cmd[i] = std::toupper(cmd[i]);

	// Commands that don't require authentication
	if (cmd == CMD_CAP) {
		_handleCap(client_fd, params);
	}
	else if (cmd == CMD_PASS) {
		_handlePass(client_fd, params);
	}
	else if (cmd == CMD_QUIT) {
		_handleQuit(client_fd, params);
	}
	// Commands that require authentication
	else if (!_clients[client_fd].isAuthenticated()) {
		_sendToClient(client_fd, ERR_PASSWDMISMATCH());
	}
	else if (cmd == CMD_NICK) {
		_handleNick(client_fd, params);
	}
	else if (cmd == CMD_USER) {
		_handleUser(client_fd, params);
	}
	// Commands that require registration
	else if (!_clients[client_fd].isRegistered()) {
		_sendToClient(client_fd, ERR_NOTREGISTERED());
	}
	else if (cmd == CMD_JOIN) {
		_handleJoin(client_fd, params);
	}
	else if (cmd == CMD_PART) {
		_handlePart(client_fd, params);
	}
	else if (cmd == CMD_PRIVMSG) {
		_handlePrivmsg(client_fd, params);
	}
	else if (cmd == CMD_TOPIC) {
		_handleTopic(client_fd, params);
	}
	else if (cmd == CMD_KICK) {
		_handleKick(client_fd, params);
	}
	else if (cmd == CMD_INVITE) {
		_handleInvite(client_fd, params);
	}
	else if (cmd == CMD_MODE) {
		_handleMode(client_fd, params);
	}
	else if (cmd == "DCC") {
		_handleDccSend(client_fd, params);
	}
	else {
		_sendToClient(client_fd, ERR_UNKNOWNCOMMAND(cmd));
	}
}

std::vector<std::string> Server::_splitCommand(const std::string& command) {
	std::vector<std::string> tokens;
	std::istringstream iss(command);
	std::string token;

	while (iss >> token) {
		if (token[0] == ':') {
			// Rest of the line is a single parameter
			std::string rest;
			std::getline(iss, rest);
			tokens.push_back(token.substr(1) + rest);
			break;
		}
		tokens.push_back(token);
	}

	return tokens;
}

//
/* Pre-authentication commands */
//

void Server::_handleCap(int client_fd, const std::vector<std::string>& params) {
	// irssi and other modern clients send CAP LS to negotiate capabilities
	// We don't support any capabilities, so just respond with an empty list
	// and tell the client that capability negotiation is done
	
	if (params.empty()) {
		return;
	}
	
	std::string subcmd = params[0];
	
	// Convert to uppercase for comparison
	for (size_t i = 0; i < subcmd.length(); i++)
		subcmd[i] = std::toupper(subcmd[i]);
	
	if (subcmd == "LS") {
		// Client is asking what capabilities we support
		// We support none, so send an empty list
		_sendToClient(client_fd, ":server CAP * LS :\r\n");
	}
	else if (subcmd == "END") {
		// Client is done with capability negotiation
		// No action needed, client will proceed with authentication
	}
	// Ignore other CAP subcommands (REQ, ACK, NAK, etc.)
}

//
/* Authentication commands */
//

void Server::_handlePass(int client_fd, const std::vector<std::string>& params) {
	// Already Registered
	if (_clients[client_fd].isRegistered()) {
		_sendToClient(client_fd, ERR_ALREADYREGISTRED());
		return;
	}
	
	// No params
	if (params.empty()) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_PASS));
		return;
	}

	// Authenticate pass if correct, else error
	if (params[0] == _password) {
		_clients[client_fd].setAuthenticated(true);
	} else {
		_sendToClient(client_fd, ERR_PASSWDMISMATCH());
		// Allow client to retry - don't disconnect
	}
}

void Server::_handleNick(int client_fd, const std::vector<std::string>& params) {
	if (params.empty()) {
		_sendToClient(client_fd, ERR_NONICKNAMEGIVEN());
		return;
	}

	std::string new_nick = params[0];
	
	// Check if nickname is already in use
	if (_isNicknameInUse(new_nick, client_fd)) {
		std::string current_nick = _clients[client_fd].getNickname();
		if (current_nick.empty()) {
			current_nick = "*";
		}
		_sendToClient(client_fd, ERR_NICKNAMEINUSE(current_nick, new_nick));
		return;
	}

	std::string old_nick = _clients[client_fd].getNickname();
	_clients[client_fd].setNickname(new_nick);

	if (!old_nick.empty()) {
		// Notify user of nick change
		_sendToClient(client_fd, ":" + old_nick + " NICK :" + new_nick + "\r\n");
	}

	// Register
	_handleRegister(client_fd);
}

void Server::_handleUser(int client_fd, const std::vector<std::string>& params) {
	if (params.size() < 4) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_USER));
		return;
	}

	if (_clients[client_fd].isRegistered()) {
		_sendToClient(client_fd, ERR_ALREADYREGISTRED());
		return;
	}

	_clients[client_fd].setUsername(params[0]);
	_clients[client_fd].setRealname(params[3]);
	
	// Register
	_handleRegister(client_fd);
}

void Server::_handleRegister(int client_fd) {
	// Already registered
	if (_clients[client_fd].isRegistered()) {
		return;
	}

	// Try Register
	if (!_clients[client_fd].getUsername().empty() &&
		!_clients[client_fd].getNickname().empty()) {
		_clients[client_fd].setRegistered(true);
		
		std::string nick = _clients[client_fd].getNickname();
		// Send welcome messages (RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO)
		_sendToClient(client_fd, RPL_WELCOME(nick));
		_sendToClient(client_fd, RPL_YOURHOST(nick));
		_sendToClient(client_fd, RPL_CREATED(nick));
		_sendToClient(client_fd, RPL_MYINFO(nick));
	}
}

//
/* Channel commands */
//

void Server::_handleJoin(int client_fd, const std::vector<std::string>& params) {
	if (params.empty()) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_JOIN));
		return;
	}

	std::string channel_name = params[0];
	
	// Check if channel name is empty or invalid
	if (channel_name.empty() || channel_name[0] != '#') {
		_sendToClient(client_fd, ERR_NOSUCHCHANNEL(channel_name));
		return;
	}
	
	std::string key = params.size() > 1 ? params[1] : "";

	Channel* channel = _getChannel(channel_name);
	if (!channel) {
		// Create new channel and make user operator
		channel = _createChannel(channel_name);
		channel->addOperator(client_fd);
	}

	// Check if already in channel
	if (channel->isMember(client_fd)) {
		return;
	}

	// Check invite-only
	if (channel->isInviteOnly() && !channel->isInvited(client_fd)) {
		_sendToClient(client_fd, ERR_INVITEONLYCHAN(channel_name));
		return;
	}

	// Check key
	if (!channel->getKey().empty() && channel->getKey() != key) {
		_sendToClient(client_fd, ERR_BADCHANNELKEY(channel_name));
		return;
	}

	// Check user limit
	if (channel->isFull()) {
		_sendToClient(client_fd, ERR_CHANNELISFULL(channel_name));
		return;
	}

	// Join channel
	channel->addMember(client_fd);
	channel->removeInvite(client_fd); // Remove invite if was invited

	// Send JOIN to all members
	std::string join_msg = ":" + _getClientPrefix(client_fd) + " JOIN " + channel_name + "\r\n";
	_sendToChannel(channel, join_msg, -1);

	// Send topic if exists
	if (!channel->getTopic().empty()) {
		_sendToClient(client_fd, RPL_TOPIC(_clients[client_fd].getNickname(), channel_name, channel->getTopic()));
	} else {
		_sendToClient(client_fd, RPL_NOTOPIC(_clients[client_fd].getNickname(), channel_name));
	}

	// Send names list (simplified)
	std::string names = "";
	const std::set<int>& members = channel->getMembers();
	for (std::set<int>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (channel->isOperator(*it))
			names += "@";
		names += _clients[*it].getNickname() + " ";
	}
	_sendToClient(client_fd, RPL_NAMREPLY(_clients[client_fd].getNickname(), channel_name, names));
	_sendToClient(client_fd, RPL_ENDOFNAMES(_clients[client_fd].getNickname(), channel_name));
}

void Server::_handlePart(int client_fd, const std::vector<std::string>& params) {
	if (params.empty()) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_PART));
		return;
	}

	std::string channel_name = params[0];
	std::string reason = params.size() > 1 ? params[1] : "Leaving";

	Channel* channel = _getChannel(channel_name);
	if (!channel) {
		_sendToClient(client_fd, ERR_NOSUCHCHANNEL(channel_name));
		return;
	}

	if (!channel->isMember(client_fd)) {
		_sendToClient(client_fd, ERR_NOTONCHANNEL(channel_name));
		return;
	}

	// Send PART to all members
	std::string part_msg = ":" + _getClientPrefix(client_fd) + " PART " + channel_name + " :" + reason + "\r\n";
	_sendToChannel(channel, part_msg, -1);

	channel->removeMember(client_fd);

	// Delete channel if empty
	if (channel->isEmpty()) {
		delete channel;
		_channels.erase(channel_name);
	}
}

void Server::_handlePrivmsg(int client_fd, const std::vector<std::string>& params) {
	if (params.size() < 2) {
		_sendToClient(client_fd, ERR_NORECIPIENT(CMD_PRIVMSG));
		return;
	}

	std::string target = params[0];
	std::string message = params[1];

	// Detect DCC SEND
	if (message.find("\001DCC SEND") == 0) {
		// Client is initiating a file transfer
		// Just forward it to the recipient
		std::string msg = ":" + _getClientPrefix(client_fd) + " PRIVMSG " + target + " :" + message + "\r\n";

		if (target[0] == '#') {
			Channel* channel = _getChannel(target);
			if (!channel) {
				_sendToClient(client_fd, ERR_NOSUCHCHANNEL(target));
				return;
			}
			if (!channel->isMember(client_fd)) {
				_sendToClient(client_fd, ERR_CANNOTSENDTOCHAN(target));
				return;
			}
		} else {
			int target_fd = _getClientFdByNickname(target);
			if (target_fd == -1) {
				_sendToClient(client_fd, ERR_NOSUCHNICK(target));
				return;
			}
			_sendToClient(target_fd, msg);
		}
		return;
	}
	std::string msg = ":" + _getClientPrefix(client_fd) + " PRIVMSG " + target + " :" + message + "\r\n";

	// Check if target is a channel
	if (target[0] == '#') {
		Channel* channel = _getChannel(target);
		if (!channel) {
			_sendToClient(client_fd, ERR_NOSUCHCHANNEL(target));
			return;
		}

		if (!channel->isMember(client_fd)) {
			_sendToClient(client_fd, ERR_CANNOTSENDTOCHAN(target));
			return;
		}

		_sendToChannel(channel, msg, client_fd);
	} else {
		// Private message to user
		int target_fd = _getClientFdByNickname(target);
		if (target_fd == -1) {
			_sendToClient(client_fd, ERR_NOSUCHNICK(target));
			return;
		}

		_sendToClient(target_fd, msg);
	}
}

void Server::_handleTopic(int client_fd, const std::vector<std::string>& params) {
	if (params.empty()) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_TOPIC));
		return;
	}

	std::string channel_name = params[0];
	Channel* channel = _getChannel(channel_name);
	
	if (!channel) {
		_sendToClient(client_fd, ERR_NOSUCHCHANNEL(channel_name));
		return;
	}

	if (!channel->isMember(client_fd)) {
		_sendToClient(client_fd, ERR_NOTONCHANNEL(channel_name));
		return;
	}

	// View topic
	if (params.size() == 1) {
		if (channel->getTopic().empty()) {
			_sendToClient(client_fd, RPL_NOTOPIC(_clients[client_fd].getNickname(), channel_name));
		} else {
			_sendToClient(client_fd, RPL_TOPIC(_clients[client_fd].getNickname(), channel_name, channel->getTopic()));
		}
		return;
	}

	// Set topic
	if (channel->isTopicRestricted() && !channel->isOperator(client_fd)) {
		_sendToClient(client_fd, ERR_CHANOPRIVSNEEDED(channel_name));
		return;
	}

	std::string new_topic = params[1];
	channel->setTopic(new_topic);

	// Notify all members
	std::string topic_msg = ":" + _getClientPrefix(client_fd) + " TOPIC " + channel_name + " :" + new_topic + "\r\n";
	_sendToChannel(channel, topic_msg, -1);
}

void Server::_handleKick(int client_fd, const std::vector<std::string>& params) {
	if (params.size() < 2) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_KICK));
		return;
	}

	std::string channel_name = params[0];
	std::string target_nick = params[1];
	std::string reason = params.size() > 2 ? params[2] : "Kicked";

	Channel* channel = _getChannel(channel_name);
	if (!channel) {
		_sendToClient(client_fd, ERR_NOSUCHCHANNEL(channel_name));
		return;
	}

	if (!channel->isOperator(client_fd)) {
		_sendToClient(client_fd, ERR_CHANOPRIVSNEEDED(channel_name));
		return;
	}

	int target_fd = _getClientFdByNickname(target_nick);
	if (target_fd == -1 || !channel->isMember(target_fd)) {
		_sendToClient(client_fd, ERR_USERNOTINCHANNEL(target_nick, channel_name));
		return;
	}

	// Send KICK to all members
	std::string kick_msg = ":" + _getClientPrefix(client_fd) + " KICK " + channel_name + " " + target_nick + " :" + reason + "\r\n";
	_sendToChannel(channel, kick_msg, -1);

	channel->removeMember(target_fd);
}

void Server::_handleInvite(int client_fd, const std::vector<std::string>& params) {
	if (params.size() < 2) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_INVITE));
		return;
	}

	std::string target_nick = params[0];
	std::string channel_name = params[1];

	Channel* channel = _getChannel(channel_name);
	if (!channel) {
		_sendToClient(client_fd, ERR_NOSUCHCHANNEL(channel_name));
		return;
	}

	if (!channel->isMember(client_fd)) {
		_sendToClient(client_fd, ERR_NOTONCHANNEL(channel_name));
		return;
	}

	if (channel->isInviteOnly() && !channel->isOperator(client_fd)) {
		_sendToClient(client_fd, ERR_CHANOPRIVSNEEDED(channel_name));
		return;
	}

	int target_fd = _getClientFdByNickname(target_nick);
	if (target_fd == -1) {
		_sendToClient(client_fd, ERR_NOSUCHNICK(target_nick));
		return;
	}

	if (channel->isMember(target_fd)) {
		_sendToClient(client_fd, ERR_USERONCHANNEL(target_nick, channel_name));
		return;
	}

	channel->addInvite(target_fd);

	// Notify inviter
	_sendToClient(client_fd, RPL_INVITING(_clients[client_fd].getNickname(), target_nick, channel_name));

	// Send invite to target
	std::string invite_msg = ":" + _getClientPrefix(client_fd) + " INVITE " + target_nick + " :" + channel_name + "\r\n";
	_sendToClient(target_fd, invite_msg);
}

void Server::_handleMode(int client_fd, const std::vector<std::string>& params) {
	if (params.empty()) {
		_sendToClient(client_fd, ERR_NEEDMOREPARAMS(CMD_MODE));
		return;
	}

	std::string channel_name = params[0];
	Channel* channel = _getChannel(channel_name);
	
	if (!channel) {
		_sendToClient(client_fd, ERR_NOSUCHCHANNEL(channel_name));
		return;
	}

	// View modes
	if (params.size() == 1) {
		std::string modes = "+";
		if (channel->isInviteOnly()) modes += "i";
		if (channel->isTopicRestricted()) modes += "t";
		if (!channel->getKey().empty()) modes += "k";
		if (channel->getUserLimit() > 0) modes += "l";
		
		_sendToClient(client_fd, RPL_CHANNELMODEIS(_clients[client_fd].getNickname(), channel_name, modes));
		return;
	}

	// Set modes
	if (!channel->isOperator(client_fd)) {
		_sendToClient(client_fd, ERR_CHANOPRIVSNEEDED(channel_name));
		return;
	}

	std::string modestring = params[1];
	bool adding = true;
	size_t param_idx = 2;

	for (size_t i = 0; i < modestring.length(); i++) {
		char mode = modestring[i];
		
		if (mode == '+') {
			adding = true;
		} else if (mode == '-') {
			adding = false;
		} else if (mode == 'i') {
			channel->setInviteOnly(adding);
		} else if (mode == 't') {
			channel->setTopicRestricted(adding);
		} else if (mode == 'k') {
			if (adding && param_idx < params.size()) {
				channel->setKey(params[param_idx++]);
			} else if (!adding) {
				channel->setKey("");
			}
		} else if (mode == 'o') {
			if (param_idx < params.size()) {
				std::string target_nick = params[param_idx++];
				int target_fd = _getClientFdByNickname(target_nick);
				
				if (target_fd != -1 && channel->isMember(target_fd)) {
					if (adding) {
						channel->addOperator(target_fd);
					} else {
						channel->removeOperator(target_fd);
					}
				}
			}
		} else if (mode == 'l') {
			if (adding && param_idx < params.size()) {
				std::istringstream iss(params[param_idx++]);
				int limit;
				iss >> limit;
				channel->setUserLimit(limit);
			} else if (!adding) {
				channel->setUserLimit(0);
			}
		} else {
			_sendToClient(client_fd, ERR_UNKNOWNMODE(mode));
		}
	}

	// Notify channel of mode change
	std::string mode_msg = ":" + _getClientPrefix(client_fd) + " MODE " + channel_name + " " + modestring;
	for (size_t i = 2; i < params.size(); i++) {
		mode_msg += " " + params[i];
	}
	mode_msg += "\r\n";
	_sendToChannel(channel, mode_msg, -1);
}

void Server::_handleQuit(int client_fd, const std::vector<std::string>& params) {
	std::string reason = params.empty() ? "Client quit" : params[0];
	
	// Notify all channels
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second->isMember(client_fd)) {
			std::string quit_msg = ":" + _getClientPrefix(client_fd) + " QUIT :" + reason + "\r\n";
			_sendToChannel(it->second, quit_msg, client_fd);
			it->second->removeMember(client_fd);
		}
	}

	_removeClient(client_fd);
}

// ==================== HELPER FUNCTIONS ====================

void Server::_sendToClient(int client_fd, const std::string& message) {
	// Buffer the message instead of sending directly
	_output_buffers[client_fd] += message;

	// Update poll events to include POLLOUT
	for (size_t i = 0; i < _poll_fds.size(); i++) {
		if (_poll_fds[i].fd == client_fd) {
			_poll_fds[i].events |= POLLOUT;
			break;
		}
	}
}

void Server::_flushClientBuffer(int client_fd) {
	std::string& buffer = _output_buffers[client_fd];
	if (buffer.empty()) {
		return;
	}

	ssize_t sent = send(client_fd, buffer.c_str(), buffer.length(), 0);
	if (sent > 0) {
		buffer.erase(0, static_cast<size_t>(sent));
	} else if (sent == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		// Real error - will be caught by POLLERR/POLLHUP
		return;
	}

	// If buffer is empty, remove POLLOUT from events
	if (buffer.empty()) {
		for (size_t i = 0; i < _poll_fds.size(); i++) {
			if (_poll_fds[i].fd == client_fd) {
				_poll_fds[i].events = POLLIN;
				break;
			}
		}
	}
}

void Server::_sendToChannel(Channel* channel, const std::string& message, int exclude_fd) {
	if (!channel) {
		return;
	}

	const std::set<int>& members = channel->getMembers();
	for (std::set<int>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (*it != exclude_fd) {
			_sendToClient(*it, message);
		}
	}
}

void Server::_removeClient(int client_fd) {
	// Remove from all channels
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ) {
		if (it->second->isMember(client_fd)) {
			it->second->removeMember(client_fd);
			if (it->second->isEmpty()) {
				delete it->second;
				_channels.erase(it++);
				continue;
			}
		}
		++it;
	}

	// Remove from poll_fds
	for (std::vector<struct pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it) {
		if (it->fd == client_fd) {
			_poll_fds.erase(it);
			break;
		}
	}

	// Remove from clients map
	_clients.erase(client_fd);

	// Also remove output buffer
	_output_buffers.erase(client_fd);

	close(client_fd);
}

bool Server::_isNicknameInUse(const std::string& nickname, int exclude_fd) const {
	for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->first != exclude_fd && it->second.getNickname() == nickname) {
			return true;
		}
	}
	return false;
}

int Server::_getClientFdByNickname(const std::string& nickname) const {
	for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second.getNickname() == nickname) {
			return it->first;
		}
	}
	return -1;
}

std::string Server::_getClientPrefix(int client_fd) const {
	const Client& client = _clients.find(client_fd)->second;
	return client.getNickname() + "!" + client.getUsername() + "@localhost";
}

Channel* Server::_getChannel(const std::string& name) {
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end()) {
		return it->second;
	}
	return NULL;
}

Channel* Server::_createChannel(const std::string& name) {
	Channel* channel = new Channel(name);
	_channels[name] = channel;
	return channel;
}

static std::string intToString(int value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

void Server::_handleDccSend(int client_fd, const std::vector<std::string>& params) {
	// DCC SEND <filename> <target_nick>
	if (params.size() < 3 || params[0] != "SEND") {
		_sendToClient(client_fd, "ERROR: Invalid DCC SEND syntax\r\n");
		return;
	}

	std::string filename = params[1];
	std::string target_nick = params[2];

	// Check if the target exists
	int target_fd = _getClientFdByNickname(target_nick);
	if (target_fd == -1) {
		_sendToClient(client_fd, ERR_NOSUCHNICK(target_nick));
		return;
	}

	// Create transfer
	std::string sender_nick = _clients[client_fd].getNickname();
	FileTransfer* transfer = new FileTransfer(filename, sender_nick, target_nick);

	int port = 0;
	if (!transfer->setupListenSocket(port)) {
		_sendToClient(client_fd, "ERROR: Failed to setup file transfer\r\n");
		delete transfer;
		return;
	}

	// Generate DCC message
	std::string dccmsg = transfer->generateDccSendMessage(port, _getClientPrefix(client_fd));

	// Send to recipient via PRIVMSG
	_sendToClient(target_fd, dccmsg);

	//Store active transfers
	_active_transfers[filename] = transfer;

	// Confirm to sender
	std::string confirm = ":server NOTICE " + sender_nick + 
							" :DCC SEND initiated for " + filename +
							" on port " + intToString(port) + "\r\n";
	_sendToClient(client_fd, confirm);
	
}

void Server::_registerTransferFds() {
	for (std::map<std::string, FileTransfer*>::iterator it = _active_transfers.begin();
		 it != _active_transfers.end(); ++it) {
		FileTransfer* transfer = it->second;

		int listen_fd = transfer->getListenFd();
		int transfer_fd = transfer->getTransferFd();

		// Check if listen_fd needs to be added (for accept)
		if (listen_fd >= 0) {
			bool found = false;
			for (size_t i = 0; i < _poll_fds.size(); i++) {
				if (_poll_fds[i].fd == listen_fd) {
					found = true;
					break;
				}
			}
			if (!found) {
				struct pollfd pfd;
				pfd.fd = listen_fd;
				pfd.events = POLLIN;
				pfd.revents = 0;
				_poll_fds.push_back(pfd);
			}
		}

		// Check if transfer_fd needs to be added (for send)
		if (transfer_fd >= 0) {
			bool found = false;
			for (size_t i = 0; i < _poll_fds.size(); i++) {
				if (_poll_fds[i].fd == transfer_fd) {
					found = true;
					break;
				}
			}
			if (!found) {
				struct pollfd pfd;
				pfd.fd = transfer_fd;
				pfd.events = POLLOUT;
				pfd.revents = 0;
				_poll_fds.push_back(pfd);
			}
		}
	}

	// Remove stale transfer FDs from poll
	for (std::vector<struct pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ) {
		bool is_server = (it->fd == _server_fd);
		bool is_client = (_clients.find(it->fd) != _clients.end());
		bool is_transfer = false;

		for (std::map<std::string, FileTransfer*>::iterator tit = _active_transfers.begin();
			 tit != _active_transfers.end(); ++tit) {
			if (tit->second->getListenFd() == it->fd ||
				tit->second->getTransferFd() == it->fd) {
				is_transfer = true;
				break;
			}
		}

		if (!is_server && !is_client && !is_transfer) {
			it = _poll_fds.erase(it);
		} else {
			++it;
		}
	}
}

// Remove _processDccTransfers() - no longer needed, handled in main loop

void Server::_cleanupCompletedTransfers() {
	std::map<std::string, FileTransfer*>::iterator it = _active_transfers.begin();
	while (it != _active_transfers.end()) {
		if (!it->second->isActive() && it->second->getListenFd() < 0 && it->second->getTransferFd() < 0) {
			delete it->second;
			_active_transfers.erase(it++);
		} else {
			++it;
		}
	}
}