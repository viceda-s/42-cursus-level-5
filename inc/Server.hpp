#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/poll.h>
#include <vector>
#include <map>

#define LISTEN_CONN 10

#include "Client.hpp"
#include "Channel.hpp"

class Server {
	int _port;
	std::string _password;
	int	_server_fd;
	std::vector<struct pollfd> _poll_fds;
	std::map<int, Client> _clients; // fd -> Client
	std::map<std::string, Channel*> _channels; // name -> Channel*

public:
	Server(int port, const std::string& password);
	~Server();

	void run(); // Main loop

private:
	// Socket
	void _setupSocket();
	void _acceptNewClient();
	void _handleClientData(int client_fd);
	void _setNonBlocking(int fd);

	// Command processing
	void _processClientCommands(int client_fd);
	void _executeCommand(int client_fd, const std::string& command);
	std::vector<std::string> _splitCommand(const std::string& command);

	// Pre-authentication commands
	void _handleCap(int client_fd, const std::vector<std::string>& params);
	void _handlePass(int client_fd, const std::vector<std::string>& params);

	// Authentication commands
	void _handleNick(int client_fd, const std::vector<std::string>& params);
	void _handleUser(int client_fd, const std::vector<std::string>& params);
	void _handleRegister(int client_fd);

	// Channel commands
	void _handleJoin(int client_fd, const std::vector<std::string>& params);
	void _handlePart(int client_fd, const std::vector<std::string>& params);
	void _handleTopic(int client_fd, const std::vector<std::string>& params);
	void _handleKick(int client_fd, const std::vector<std::string>& params);
	void _handleInvite(int client_fd, const std::vector<std::string>& params);
	void _handleMode(int client_fd, const std::vector<std::string>& params);

	// Message commands
	void _handlePrivmsg(int client_fd, const std::vector<std::string>& params);
	void _handleQuit(int client_fd, const std::vector<std::string>& params);

	// Helpers
	void _sendToClient(int client_fd, const std::string& message);
	void _sendToChannel(Channel* channel, const std::string& message, int exclude_fd);
	void _removeClient(int client_fd);
	bool _isNicknameInUse(const std::string& nickname, int exclude_fd) const;
	int _getClientFdByNickname(const std::string& nickname) const;
	std::string _getClientPrefix(int client_fd) const;
	Channel* _getChannel(const std::string& name);
	Channel* _createChannel(const std::string& name);
};

#endif // SERVER_HPP