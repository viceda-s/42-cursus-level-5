#include "Channel.hpp"

Channel::Channel(const std::string& name) 
	: _name(name), _inviteOnly(false), _topicRestricted(true), _userLimit(0) {}

Channel::~Channel() {}

// Getters
std::string Channel::getName() const { return _name; }
std::string Channel::getTopic() const { return _topic; }
std::string Channel::getKey() const { return _key; }
bool Channel::isInviteOnly() const { return _inviteOnly; }
bool Channel::isTopicRestricted() const { return _topicRestricted; }
int Channel::getUserLimit() const { return _userLimit; }
const std::set<int>& Channel::getMembers() const { return _members; }
const std::set<int>& Channel::getOperators() const { return _operators; }

// Setters
void Channel::setTopic(const std::string& topic) { _topic = topic; }
void Channel::setKey(const std::string& key) { _key = key; }
void Channel::setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly; }
void Channel::setTopicRestricted(bool topicRestricted) { _topicRestricted = topicRestricted; }
void Channel::setUserLimit(int limit) { _userLimit = limit; }

// Member management
void Channel::addMember(int client_fd) { _members.insert(client_fd); }
void Channel::removeMember(int client_fd) { 
	_members.erase(client_fd);
	_operators.erase(client_fd);
}
bool Channel::isMember(int client_fd) const { 
	return _members.find(client_fd) != _members.end(); 
}

// Operator management
void Channel::addOperator(int client_fd) { _operators.insert(client_fd); }
void Channel::removeOperator(int client_fd) { _operators.erase(client_fd); }
bool Channel::isOperator(int client_fd) const { 
	return _operators.find(client_fd) != _operators.end(); 
}

// Invite management
void Channel::addInvite(int client_fd) { _invitedUsers.insert(client_fd); }
void Channel::removeInvite(int client_fd) { _invitedUsers.erase(client_fd); }
bool Channel::isInvited(int client_fd) const { 
	return _invitedUsers.find(client_fd) != _invitedUsers.end(); 
}

// Checks
bool Channel::isEmpty() const { return _members.empty(); }
bool Channel::isFull() const { 
	return _userLimit > 0 && static_cast<int>(_members.size()) >= _userLimit; 
}
