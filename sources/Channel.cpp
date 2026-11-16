/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:10:47 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/16 16:25:30 by malaimo          ###   ########.fr       */
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

std::vector<BannedMember>	Channel::getBanList() const 
{
	return (_banList);
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

void		Channel::setOperator(Client &member)
{
	_members[member.getFd()].op = true;
}


void		Channel::addtoBanList(std::string nick, std::string user, std::string host)
{
	BannedMember	newban;
	
	newban.nick = nick;
	newban.user = user;
	newban.host = host;
	newban.banid = nick + user + host;
	_banList.push_back(newban);
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
		if (checkNick(it->nick, member.getNickname()) && checkUser(it->user, member.getUsername()) && checkHost(it->host, member.getHostname()))
			return (true);
	}
	if (DEBUG)
	{
		std::cout << GREEN "IS NOT BANNED" << std::endl;
	}
	return (false);
}

bool	Channel::checkNick(std::string bannedNick, std::string clientNick)
{
	if (DEBUG)
		std::cout << RED "bannedNick = " RESET << bannedNick << RED " clientNick = " RESET << clientNick << std::endl;
	if (bannedNick == "*")
		return true;
	
	if (DEBUG)
		std::cout << "aled" << std::endl;
	std::string temp;
	size_t		itClient = 0;
	for (size_t i = 0; i < bannedNick.size(); i++)
	{
		if (DEBUG)
			std::cout << "entered boucle" << std::endl;
		if (bannedNick[i] == '*')
			continue;
		if (bannedNick[i] == '?')
		{
			if (itClient >= clientNick.size())
				return true;
			itClient++;
			continue;
		}
		if (DEBUG)
			std::cout << "entered find" << std::endl;
		size_t stop = bannedNick.find_first_of("*?", i);
		if (stop == std::string::npos)
			temp = bannedNick.substr(i);
		else
			temp = bannedNick.substr(i, stop);
		if (!clientNick.find(temp, itClient))
			return true;
		if (DEBUG)
			std::cout << "temp = " << temp << " clientnickfind = " << clientNick.find(temp, itClient) << " " << clientNick.compare(temp) << std::endl;
		itClient += temp.size();
		i += temp.size();
	}
	if (DEBUG)
		std::cout << GREEN "Nick not banned" RESET << std::endl;
	return false;
}

bool	Channel::checkUser(std::string bannedUser, std::string clientUser)
{
	if (DEBUG)
		std::cout << RED "bannedUser = " RESET << bannedUser << std::endl;
	if (bannedUser == "!*")
		return true;
	
	std::string temp;
	size_t		itClient = 0;
	for (size_t i = 1; i < bannedUser.size(); i++)
	{
		if (bannedUser[i] == '*')
			continue;
		if (bannedUser[i] == '?')
		{
			if (itClient >= clientUser.size())
				return true;
			itClient++;
			continue;
		}
		size_t stop = bannedUser.find_first_of("*?", i);
		if (stop == std::string::npos)
			temp = bannedUser.substr(i);
		else
			temp = bannedUser.substr(i, stop);
		if (!clientUser.find(temp, itClient))
			return true;
		itClient += temp.size();
		i += temp.size();	
	}
	if (DEBUG)
		std::cout << GREEN "User not banned" RESET << std::endl;
	return false;
}


bool	Channel::checkHost(std::string bannedHost, std::string clientHost)
{
	if (DEBUG)
		std::cout << RED "bannedHost = " RESET << bannedHost << std::endl;
	if (bannedHost == "@*")
		return true;
	
	std::string temp;
	size_t		itClient = 0;
	for (size_t i = 1; i < bannedHost.size(); i++)
	{
		if (bannedHost[i] == '*')
			continue;
		if (bannedHost[i] == '?')
		{
			if (itClient >= clientHost.size())
				return true;
			itClient++;
			continue;
		}
		size_t stop = bannedHost.find_first_of("*?", i);
		if (stop == std::string::npos)
			temp = bannedHost.substr(i);
		else
			temp = bannedHost.substr(i, stop);
		if (!clientHost.find(temp, itClient))
			return true;
		itClient += temp.size();
		i += temp.size();	
	}
	if (DEBUG)
		std::cout << GREEN "Host not banned" RESET << std::endl;
	return false;
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
