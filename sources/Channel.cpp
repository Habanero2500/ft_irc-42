/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:10:47 by malaimo           #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2026/11/12 18:36:35 by jmetayer         ###   ########.fr       */
=======
/*   Updated: 2026/11/12 18:15:32 by malaimo          ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Channel.hpp"
#include "../includes/class/Server.hpp"

Channel::Channel() : _name(""), _maxMembers(1000), _pwd("")
{
}

Channel::Channel(std::string name) : _name(name), _maxMembers(1000), _pwd("")
{
}

Channel::Channel(const Channel &copy) : _name(copy._name), _members(copy._members), _banList(copy._banList), _maxMembers(copy._maxMembers), _pwd(copy._pwd)
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

std::string	Channel::getModes() const
{
	std::string	levels[8] = {"P", "S", "I", "T", "N", "M", "L", "K"};
	std::string	activeModes = "+";

	for (size_t i = 0; i < 8; i++)
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

void		Channel::setPwd(std::string &pwd)
{
	_pwd = pwd;
}

void		Channel::setmode(int mode)
{
	_modes.set(mode);
}

void		Channel::setMaxMembers(int max)
{
	_maxMembers = max;
}

/****** MEMBERS MANAGEMENT *******/

void		Channel::addMember(Client &member)
{
	_members[member.getFd()].client = &member;
	_members[member.getFd()].op = false;
    _members[member.getFd()].voice = false;
}

void		Channel::setOperator(Client &member)
{
	_members[member.getFd()].op = true;
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

bool	Channel::isMemberBanned(Client &member)
{
	for (std::vector<BannedMember>::iterator it = _banList.begin(); it != _banList.end(); it++)
	{
		if (it->nickname == member.getNickname() || it->username == member.getUsername() || it->hostname == member.getHostname())
			return (true);
	}
	return (false);
}

bool	Channel::isMember(Client &member)
{
	for (std::map<int, Member>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (it->second.client->getNickname() == member.getNickname())
			return true;
	}
	return false;
}

bool	Channel::isOperator(Client &member)
{
	if (!isMember(member))
		return false;
	if (_members[member.getFd()].op)
		return true;
	return false;
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
	for (std::vector<int>::iterator it = _chouffins.begin(); it != _chouffins.end(); ++it)
		if (*it == client.getFd()) {
			_chouffins.erase(it);
			return ;//Voir si il faut modifier plus de trucs
		}
}
