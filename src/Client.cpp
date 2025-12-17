#include "Client.hpp"

Client::Client() : _fd(-1), _authenticated(false), _registered(false) {}

Client::Client(int fd) : _fd(fd), _authenticated(false), _registered(false) {}

Client::~Client() {}

// Getters
int Client::getFd() const { return _fd; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getRealname() const { return _realname; }
std::string& Client::getBuffer() { return _buffer; }
bool Client::isAuthenticated() const { return _authenticated; }
bool Client::isRegistered() const { return _registered; }

// Setters
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setRealname(const std::string& realname) { _realname = realname; }
void Client::setAuthenticated(bool authenticated) { _authenticated = authenticated; }
void Client::setRegistered(bool registered) { _registered = registered; }

// Buffer management
void Client::addToBuffer(const std::string& data) { _buffer += data; }
void Client::clearBuffer() { _buffer.clear(); }