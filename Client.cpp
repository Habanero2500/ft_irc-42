/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:28:02 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/02 15:16:48 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

// **** ORTHODOX CANONICAL FORM ****

Client::Client(void) : _nickname(""), _username(""), _hostname(""), _realname(""), _ID(""), _fd(-1), _buffer("") {}

Client::Client(const Client& copy) : _nickname(copy._nickname), _username(copy._username), _hostname(copy._hostname), _realname(copy._realname), _ID(copy._ID), _fd(copy._fd), _adress(copy._adress), _pollFd(copy._pollFd), _buffer(copy._buffer) {}

Client& Client::operator=(const Client& copy)
{
    if(this == &copy)
        return *this;
    else
    {
        _nickname = copy._nickname;
        _username = copy._username;
        _hostname = copy._hostname;
        _realname = copy._realname;
        _ID = copy._ID;
        _fd = copy._fd;
		_adress = copy._adress;
		_pollFd = copy._pollFd;
		_buffer = copy._buffer;
    }
    return *this;
}

Client::~Client(){}

//********** SETTERS AND GETTERS ************

void setNickname(std::string name){ _nickname = name;}
void setUsername(std::string name){ _username = name;}

void setHostname(std::string name){ _hostname = name;}
void setRealName(std::string name){ _realname = name;}

void Client::setID(std::string ID)
{
	_ID = ID;
}

void Client::setFd(int i)
{
	_fd = i;
}

void Client::setPollFd()
{
	_pollFd.fd = _fd;
	_pollFd.events = POLLIN;
	_pollFd.revents = 0;
}

void Client::addBuffer(char buffer[1024])
{
	_buffer.append(buffer);
}

std::string Client::getID(void)
{
    return _ID;
}
int Client::getFd(void)
{
    return _fd;
}

struct pollfd Client::getPollFd(void)
{
    return _pollFd;
}

std::string	Client::getBuffer(void)
{
    return (_buffer);
}
std::string 	getNickname(void){return _nickname;}
std::string 	getUsername(void){return _username;}
std::string 	getHostname(void){return _hostname;}
std::string 	getRealName(void){return _realname;}

//**********INITIALISATION ************

void	Client::initClientAdress(int port)
{
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(port);
}
