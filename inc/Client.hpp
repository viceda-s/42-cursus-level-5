#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
private:
	int _fd;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _buffer;
	bool _authenticated;
	bool _registered;

public:
	Client();
	Client(int fd);
	~Client();

	// Getters
	int getFd() const;
	std::string getNickname() const;
	std::string getUsername() const;
	std::string getRealname() const;
	std::string& getBuffer();
	bool isAuthenticated() const;
	bool isRegistered() const;

	// Setters
	void setNickname(const std::string& nickname);
	void setUsername(const std::string& username);
	void setRealname(const std::string& realname);
	void setAuthenticated(bool authenticated);
	void setRegistered(bool registered);

	// Buffer management
	void addToBuffer(const std::string& data);
	void clearBuffer();
};

#endif // CLIENT_HPP