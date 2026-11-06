/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:10:47 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/06 17:47:22 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Channel.hpp"

Channel::Channel() : _name(""), _maxMembers(1000)
{
}

Channel::Channel(std::string name) : _name(name), _maxMembers(1000)
{
}

Channel::Channel(const Channel &copy) : _name(copy._name), _members(copy._members), _banList(copy._banList), _maxMembers(copy._maxMembers)
{
}

Channel	&Channel::operator=(const Channel &copy)
{
    if(this != &copy)
    {
		_name = copy._name;
		_members = copy._members;
		_maxMembers = copy._maxMembers;
    }
    return *this;
}

Channel::~Channel()
{
}


/****** GETTERS AND SETTERS *******/


std::string	Channel::getName()
{
	return _name;
}

size_t		Channel::getNumberMember()
{
	return _members.size();
}

// Member		Channel::getMember()
// {
// 	return	
// }

	size_t		getNumberMember();	
	Member		getMember();

void		Channel::setName(std::string name)
{
	_name = name;
}


/****** MEMBERS MANAGEMENT *******/

void		Channel::addMember(Client member)
{
	_members[member.getFd()].client = member;
}

void		Channel::setOperator(Client member)
{
	_members[member.getFd()].op = 1;
}


/****** MODES *******/


bool	Channel::isModeActive(int mode)
{
	return _modes.test(mode);
}

bool	Channel::isMembersLimitReached()
{
	return (_members.size() >= _maxMembers);
}

bool	Channel::isMemberBanned(Client Member)
{
	for (std::vector<BannedMember>::iterator	it = _banList.begin(); it != _banList.end(); it++)
	{
		if (it->nickname == Member.getNickname() || it->username == Member.getUsername() || it->hostname == Member.getHostname())
			return (true);
	}
	return (false);
}