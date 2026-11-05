/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:28:02 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/05 14:34:00 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Client.hpp"

// **** ORTHODOX CANONICAL FORM ****

Client::Client(void) : _nickname(""), _username(""), _hostname(""), _realname(""), _ID(""), _fd(-1), _buffer(""), _pass(false), _nick(false), _user(false), _attempt(0) {}

Client::Client(const Client& copy) : _nickname(copy._nickname), _username(copy._username), _hostname(copy._hostname), _realname(copy._realname), _ID(copy._ID), _fd(copy._fd), _adress(copy._adress), _pollFd(copy._pollFd), _buffer(copy._buffer), _pass(false), _nick(false), _user(false), _attempt(0) {}

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
        _pass = false;
        _nick = false;
        _user = false;
        _attempt = 0;
    }
    return *this;
}

Client::~Client(){}

//********** SETTERS AND GETTERS ************

void Client::setNickname(std::string name){ _nickname = name;}
void Client::setUsername(std::string name){ _username = name;}

void Client::setHostname(std::string name){ _hostname = name;}
void Client::setRealName(std::string name){ _realname = name;}

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
void    Client::setPass(void)
{
    if(_pass == false)
        _pass = true;
}
void    Client::setNick(void)
{
    if(_nick == false)
        _nick = true;
}
void    Client::setUser(void)
{
    if(_user == false)
        _user = true;
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
std::string 	Client::getNickname(void){return _nickname;}
std::string 	Client::getUsername(void){return _username;}
std::string 	Client::getHostname(void){return _hostname;}
std::string 	Client::getRealName(void){return _realname;}
int             Client::getAttempt(void){return _attempt;}
bool            Client::getPass(void){return _pass;}
bool            Client::getNick(void){return _nick;}
bool            Client::getUser(void){return _user;}

// **** OTHERS ****

void    Client::increaseAttempt(void){_attempt++;}
bool	Client::isRegistered(void)		{return _registered;}
void	Client::setRegistered(void)		{_registered = true;}
bool	Client::shouldDisconnect(void)	{return _toDisconnect;}
void	Client::setToDisconnect(void)	{_toDisconnect = true;}
void	Client::setBuffer(const std::string &s) {_buffer = s;}


//**********INITIALISATION ************

void	Client::initClientAdress(int port)
{
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(port);
}
