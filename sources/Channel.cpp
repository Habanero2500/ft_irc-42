/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:10:47 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/05 16:20:26 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() : _name("")
{
}

Channel::Channel(std::string name) : _name(name)
{
}

Channel::Channel(const Channel &copy) : _name(copy._name), _members(copy._members)
{
}

Channel	&Channel::operator=(const Channel &copy)
{
    if(this != &copy)
    {
		_name = copy._name;
		_members = copy._members;
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

void		Channel::setName(std::string name)
{
	_name = name;
}

void		Channel::addMember(Client member)
{
	_members[member.getFd()] = member;
}