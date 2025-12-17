#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Client;

class Channel {
private:
	std::string _name;
	std::string _topic;
	std::string _key; // password
	std::set<int> _members; // client fds
	std::set<int> _operators; // operator fds
	std::set<int> _invitedUsers; // invited user fds
	
	bool _inviteOnly;
	bool _topicRestricted;
	int _userLimit;

public:
	Channel(const std::string& name);
	~Channel();

	// Getters
	std::string getName() const;
	std::string getTopic() const;
	std::string getKey() const;
	bool isInviteOnly() const;
	bool isTopicRestricted() const;
	int getUserLimit() const;
	const std::set<int>& getMembers() const;
	const std::set<int>& getOperators() const;

	// Setters
	void setTopic(const std::string& topic);
	void setKey(const std::string& key);
	void setInviteOnly(bool inviteOnly);
	void setTopicRestricted(bool topicRestricted);
	void setUserLimit(int limit);

	// Member management
	void addMember(int client_fd);
	void removeMember(int client_fd);
	bool isMember(int client_fd) const;
	
	// Operator management
	void addOperator(int client_fd);
	void removeOperator(int client_fd);
	bool isOperator(int client_fd) const;

	// Invite management
	void addInvite(int client_fd);
	void removeInvite(int client_fd);
	bool isInvited(int client_fd) const;

	// Checks
	bool isEmpty() const;
	bool isFull() const;
};

#endif // CHANNEL_HPP
