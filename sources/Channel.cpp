/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:10:47 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 20:05:05 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Channel.hpp"
#include "../includes/class/Server.hpp"
#include "../includes/utils/Colors.hpp"
#include <algorithm>
#include <iostream>
#include <climits>
#include <sstream>

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
	if (DEBUG)
	{
		std::cout << YELLOW "channel :" << _name << " destructor called" RESET << std::endl;
	}
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
	// std::vector<int>::const_iterator it = std::find(_chouffins.begin(), _chouffins.end(), client.getFd());
	// return (it != _chouffins.end());
	std::map<int, Member>::const_iterator it = _members.find(client.getFd());
	if (it != _members.end() && (it->second.op))
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

std::string	Channel::getModesArg() const 
{
	std::string modeParams = "";
	if (isModeActive(L))
		modeParams.append(size_tToString(getMaxMember()) + " ");
	if (isModeActive(K))
		modeParams.append(getPwd());
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
	long long temp = std::atoll(max.c_str());
	if (max.find_first_not_of("0123456789") != std::string::npos || temp > INT_MAX || temp < INT_MIN)
		return ;
	_maxMembers = std::atoi(max.c_str());
}


/****** MEMBERS MANAGEMENT *******/

void		Channel::addMember(Client &member)
{
	_members[member.getFd()].client = &member;
	_members[member.getFd()].op = false;
	member.increaseNumberChannels();
}

bool		Channel::setOperator(Client &member)
{
	std::map<int, Member>::iterator it = _members.find(member.getFd());
	if (it == _members.end())
	{
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

bool	Channel::isEmpty(void)
{
	if(_members.size() == 0)
		return true;
	return false;
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

std::string	Channel::getNamesList() const
{
	std::string	names;
	for (std::map<int, Member>::const_iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if (!names.empty())
			names += " ";
		if (it->second.op)
			names += "@";
		names += it->second.client->getNickname();
	}
	return names;
}

/********* UTILS **********/

std::string	Channel::size_tToString(size_t src) const
{
	std::stringstream	temp;
	temp << src;
	
	std ::string 		result = temp.str(); 
	return result;
}