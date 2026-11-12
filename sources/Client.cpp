/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:28:02 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/11 14:23:40 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Client.hpp"
#include <iostream>

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
void Client::setServername(std::string name){ _serverName = name;}
void Client::setRealName(std::string name){ _realname = name;}
void Client::setID(std::string ID){_ID = ID;}
void Client::setFd(int i){_fd = i;}

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

void    		Client::setBuffer(std::string Newbuffer){_buffer = Newbuffer;}
void			Client::addBuffer(char buffer[1024]){_buffer.append(buffer);}

std::string		Client::getID(void){return _ID;}
int				Client::getFd(void) const {return _fd;}
struct pollfd	Client::getPollFd(void){return _pollFd;}
std::string		Client::getBuffer(void){return (_buffer);}
std::string 	Client::getNickname(void) const {return _nickname;}
std::string 	Client::getUsername(void) const {return _username;}
std::string 	Client::getHostname(void){return _hostname;}
std::string 	Client::getServername(void){return _serverName;}
std::string 	Client::getRealName(void){return _realname;}
int             Client::getAttempt(void){return _attempt;}
bool            Client::getPass(void){return _pass;}
bool            Client::getNick(void){return _nick;}
bool            Client::getUser(void){return _user;}
int             Client::getNumberChannels(void){return _channelsJoined;}

// **** OTHERS ****

void    Client::increaseAttempt(void){_attempt++;}
bool	Client::isRegistered(void)		{return _registered;}
void	Client::setRegistered(void)		{_registered = true;}
bool	Client::shouldDisconnect(void)	{return _toDisconnect;}
void	Client::setToDisconnect(void)	{_toDisconnect = true;}

void    Client::displayClient(void)
{
    std::cout << "Nickname : " << _nickname << std::endl;
    std::cout << "Username : " << _username << std::endl;
    std::cout << "Hostname : " << _hostname << std::endl;
    std::cout << "Servername : " << _realname << std::endl;
    std::cout << "Realname : " << _realname << std::endl;
    std::cout << "ID : " << _ID << std::endl;
    std::cout << "Fd : " << _fd << std::endl;
    std::cout << "Pass : " << _pass << std::endl;
    std::cout << "Nick : " << _nick << std::endl;
    std::cout << "User : " << _user << std::endl;
    std::cout << "Attempt : " << _attempt << std::endl;
    std::cout << "Registered : " << _registered << std::endl;
    std::cout << "To disconnect : " << _toDisconnect << std::endl;
    std::cout << "Channels joined : " << _channelsJoined << std::endl;
}
//**********INITIALISATION ************

void	Client::initClientAdress(int port)
{
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(port);
}

/****** CHANNELS *********/
void 	Client::increaseNumberChannels(void){_channelsJoined++;}

//Buffer Management
void	Client::appendToSendBuffer(const std::string& data) {
	_sendBuffer += data;
}

std::string&	Client::getSendBuffer(void) {
	return (_sendBuffer);
}

bool	Client::isSendBufferEmpty(void) const {
	return (_sendBuffer.empty());
}
