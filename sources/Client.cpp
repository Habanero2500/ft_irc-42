/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:28:02 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 17:10:41 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

// **** ORTHODOX CANONICAL FORM ****

Client::Client(void) : _name(""), _ID("")
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
}

Client::Client(const Client& copy) : _name(copy._name), _ID(copy._ID), _fd(copy._fd) {}

Client& Client::operator=(const Client& copy)
{
    if(this == &copy)
        return *this;
    else
    {
        _name = copy._name;
        _ID = copy._ID;
        _fd = copy._fd;
        
    }
    return *this;
}

Client::~Client(){}


//======================================================

void	Client::initClientAdress(int port)
{
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(port);
}


//**** SETTERS AND GETTERS ****
// void Client::setName(std::string name) : _name(name) {}

// void Client::setID(std::string ID) : _ID(ID) {}

// void Client::setFd(int i) : _fd(i) {}

// std::string Client::getName(void)
// {
//     return _name;
// }
// std::string Client::getID(void)
// {
//     return _ID;
// }
// int Client::getFd(void)
// {
//     return _fd;
// }
