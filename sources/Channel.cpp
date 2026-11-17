/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:10:47 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/17 15:32:55 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Channel.hpp"
#include "../includes/class/Server.hpp"
#include <algorithm>
#include <iostream>

Channel::Channel() : _name(""), _maxMembers(1000), _pwd("")
{
}

Channel::Channel(std::string name) : _name(name), _maxMembers(1000), _pwd("")
{
}

Channel::Channel(const Channel &copy) : _name(copy._name), _members(copy._members), _maxMembers(copy._maxMembers), _pwd(copy._pwd)
{
}

Channel	&Channel::operator=(const Channel &copy)
{
    if(this != &copy)
    {
		_name = copy._name;
		_members = copy._members;
		_maxMembers = copy._maxMembers;
		_pwd = copy._pwd;
    }
    return *this;
}

Channel::~Channel()
{
}


/****** GETTERS AND SETTERS *******/


std::string	Channel::getName() const
{
	return _name;
}

size_t		Channel::getNumberMember() const {
	return (_members.size());
}

Member*		Channel::getMemberByNick(const std::string& client) {
	for (std::map<int, Member>::iterator it = _members.begin(); it != _members.end(); ++it) {
		if (it->second.client->getNickname() == client)
			return (&(it->second));
	}
	return	(NULL);
}

bool	Channel::isTopicSet(void) const {
	if (!_topic.empty())
		return (true);
	return (false);
}

std::string	Channel::getTopic(void) const {
	return (_topic);
}

void	Channel::setTopic(const std::string& topicName) {
	_topic = topicName;
}

bool	Channel::isClientAnOperator(const Client& client) const 
{
	if (!isMember(client))
		return false;
	if (DEBUG)
	{
		std::cout << "aaaaaaaaaaaaaa" << std::endl;
	}
	// std::vector<int>::const_iterator it = std::find(_chouffins.begin(), _chouffins.end(), client.getFd());
	// return (it != _chouffins.end());
	std::map<int, Member>::const_iterator it = _members.find(client.getFd());
	if (it == _members.end())
		return false;
	if (it->second.op)
		return true;
	return false;

}

std::string	Channel::getModes() const
{
	std::string	levels[8] = {"i", "t", "l", "k"};
	std::string	activeModes = "+";

	for (size_t i = 0; i < 4; i++)
	{
		if (_modes.test(i))
			activeModes.append(levels[i]);
	}
	return activeModes;
}

size_t		Channel::getMaxMember() const 
{
	return (_maxMembers);
}

std::string	Channel::getPwd() const 
{
	return (_pwd);
}

void		Channel::setName(std::string &name)
{
	_name = name;
}

void		Channel::setPwd(const std::string &pwd)
{
	_pwd = pwd;
}

void		Channel::setmode(const int &mode) 
{
	_modes.set(mode);
}

void		Channel::removemode(const int &mode) 
{
	_modes.reset(mode);
}

void		Channel::setMaxMembers(std::string max)
{
	_maxMembers = std::atoi(max.c_str());
}


/****** MEMBERS MANAGEMENT *******/

void		Channel::addMember(Client &member)
{
	_members[member.getFd()].client = &member;
	_members[member.getFd()].op = false;
    _members[member.getFd()].voice = false;
}

bool		Channel::setOperator(Client &member)
{
	std::map<int, Member>::iterator it = _members.find(member.getFd());
	if (it == _members.end())
	{
		if (DEBUG)
		{
			std::cout << "client not a member" << std::endl;
		}
		return true;
	}
	else
		_members[member.getFd()].op = true;
	return false;
}

bool		Channel::removeOperator(Client &member)
{
	std::map<int, Member>::iterator it = _members.find(member.getFd());
	if (it == _members.end())
		return true;
	else
		_members[member.getFd()].op = false;
	return false;
}

/****** MODES *******/


bool	Channel::isModeActive(int mode) const
{
	return _modes.test(mode);
}

bool	Channel::isMembersLimitReached() const
{
	return (_members.size() >= _maxMembers);
}

bool	Channel::isMember(const Client &member) const
{
	for (std::map<int, Member>::const_iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (it->second.client->getFd() == member.getFd())
			return true;
	}
	if (DEBUG)
	{
		std::cout << "not a member" << std::endl;
	}
	return false;
}

bool	Channel::isPwdCorrect(const std::string &pwd) const
{
	if (_pwd.compare(pwd))
		return false;
	return true;
}


//Broadcast and shit
void	Channel::broadcast(Server& server, const std::string& msg) const {
	for (std::map<int, Member>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		Client* target = server.getClientByNick(it->second.client->getNickname());
		if (target)
			server.saveOrSend(*target, msg);
	}
}

void	Channel::removeClientFromChannel(Client& client) {
	_members.erase(client.getFd());
	// for (std::vector<int>::iterator it = _chouffins.begin(); it != _chouffins.end(); ++it)
	// 	if (*it == client.getFd()) {
	// 		_chouffins.erase(it);
	// 		return ;//Voir si il faut modifier plus de trucs
	// 	}
}

void	Channel::broadcastQuit(Server& server, Client& client, const std::string& msg, std::set<int>& notifiedFds) const {
	for (std::map<int, Member>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		Client* target = server.getClientByNick(it->second.client->getNickname());
		if (target && target != &client && !client.isIncluded(notifiedFds))
			server.saveOrSend(*target, msg);
	}
}

void	Channel::broadcastPrivmsg(Server& server, Client& client, const std::string& msg) const {
	for (std::map<int, Member>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		Client* target = server.getClientByNick(it->second.client->getNickname());
		if (target && target != &client)
			server.saveOrSend(*target, msg);
	}
}
