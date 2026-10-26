/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:28:02 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 14:42:48 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

// **** ORTHODOX CANONICAL FORM ****

Client::Client(void) : _name(""), _ID("") {}

Client::Client(const Client& copy) : _name(copy.getName()), _ID(copy.getID()){}

Client& Client::operator=(const Client& copy)
{
    if(this == &copy)
        return *this;
    else
    {
        _name = copy.getName();
        _ID = copy.getID();
    }
    return *this;
}

Client::~Client(){}

//**** SETTERS AND GETTERS ****
void Client::setName(std::string name) : _name(name) {}

void Client::setID(std::string ID) : _ID(ID) {}

std::string Client::getName(void)
{
    return _name;
}
std::string Client::getID(void)
{
    return _ID;
}
