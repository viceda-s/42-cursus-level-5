#ifndef IRCREPLIES_HPP
#define IRCREPLIES_HPP

// IRC Numeric Replies (RFC 2812)

// Welcome messages (001-004)
#define RPL_WELCOME(nick) (":server 001 " + (nick) + " :Welcome to the IRC Network " + (nick) + "\r\n")
#define RPL_YOURHOST(nick) (":server 002 " + (nick) + " :Your host is server, running version 1.0\r\n")
#define RPL_CREATED(nick) (":server 003 " + (nick) + " :This server was created recently\r\n")
#define RPL_MYINFO(nick) (":server 004 " + (nick) + " :server 1.0 o o\r\n")

// Channel replies (324, 331-332, 341, 353, 366)
#define RPL_CHANNELMODEIS(nick, channel, modes) (":server 324 " + (nick) + " " + (channel) + " " + (modes) + "\r\n")
#define RPL_NOTOPIC(nick, channel) (":server 331 " + (nick) + " " + (channel) + " :No topic is set\r\n")
#define RPL_TOPIC(nick, channel, topic) (":server 332 " + (nick) + " " + (channel) + " :" + (topic) + "\r\n")
#define RPL_INVITING(nick, target, channel) (":server 341 " + (nick) + " " + (target) + " " + (channel) + "\r\n")
#define RPL_NAMREPLY(nick, channel, names) (":server 353 " + (nick) + " = " + (channel) + " :" + (names) + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (":server 366 " + (nick) + " " + (channel) + " :End of /NAMES list\r\n")

// Error replies (401-482)
#define ERR_NOSUCHNICK(target) (std::string(":server 401 * ") + (target) + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(channel) (std::string(":server 403 * ") + (channel) + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(channel) (std::string(":server 404 * ") + (channel) + " :Cannot send to channel\r\n")
#define ERR_NORECIPIENT(command) (std::string(":server 411 * :No recipient given (") + (command) + ")\r\n")
#define ERR_UNKNOWNCOMMAND(command) (std::string(":server 421 * ") + (command) + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN() (std::string(":server 431 * :No nickname given\r\n"))
#define ERR_NICKNAMEINUSE(current_nick, attempted_nick) (std::string(":server 433 ") + (current_nick) + " " + (attempted_nick) + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nick, channel) (std::string(":server 441 * ") + (nick) + " " + (channel) + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(channel) (std::string(":server 442 * ") + (channel) + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nick, channel) (std::string(":server 443 * ") + (nick) + " " + (channel) + " :is already on channel\r\n")
#define ERR_NOTREGISTERED() (std::string(":server 451 * :You have not registered\r\n"))
#define ERR_NEEDMOREPARAMS(command) (std::string(":server 461 * ") + (command) + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED() (std::string(":server 462 * :You may not reregister\r\n"))
#define ERR_PASSWDMISMATCH() (std::string(":server 464 * :Password incorrect\r\n"))
#define ERR_CHANNELISFULL(channel) (std::string(":server 471 * ") + (channel) + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(mode) (std::string(":server 472 * ") + std::string(1, (mode)) + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(channel) (std::string(":server 473 * ") + (channel) + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(channel) (std::string(":server 475 * ") + (channel) + " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) (std::string(":server 482 * ") + (channel) + " :You're not channel operator\r\n")

// IRC Commands
#define CMD_CAP "CAP"
#define CMD_PASS "PASS"
#define CMD_NICK "NICK"
#define CMD_USER "USER"
#define CMD_JOIN "JOIN"
#define CMD_PART "PART"
#define CMD_PRIVMSG "PRIVMSG"
#define CMD_TOPIC "TOPIC"
#define CMD_KICK "KICK"
#define CMD_INVITE "INVITE"
#define CMD_MODE "MODE"
#define CMD_QUIT "QUIT"

#endif // IRCREPLIES_HPP
