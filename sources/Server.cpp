/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/03 11:29:18 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Replies.hpp"
#include "../includes/Client.hpp"
#include "../includes/Message.hpp"
#include "../includes/Colors.hpp"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>


// **** ORTHODOX CANONICAL FORM ****
Server::Server()
{
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("SERVER: Socket failure");
}

Server::Server(const Server &copy) : _fd(copy._fd){}//A regler

Server	&Server::operator=(const Server &copy)//A regler
{
    if(this != &copy)
    {
        _fd = copy._fd;
    }
    return *this;
}


Server::~Server()
{
	std::cout << "destructor called" << std::endl;
	close(_fd);
}

//********** MAIN FUNCTIONS ************
void	Server::initServerAdress(int port)
{
	int	opt = 1;

	std::cout << WELCOME_MESSAGE << std::endl;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);

	if (bind(_fd, (const sockaddr *)&_address, sizeof(_address)) == -1)
		throw std::runtime_error("SERVER: Bind failure");
	if (listen(_fd, 128) == -1)
		throw std::runtime_error("SERVER: Listen failure");
	
	struct pollfd	serverPollFd;
	serverPollFd.fd = _fd;
	serverPollFd.events = POLLIN;
	serverPollFd.revents = 0;
	
	_listFd.push_back(serverPollFd);
}

void	Server::pollLoop()
{
	while (true)
	{
		if (poll(&_listFd[0], _listFd.size(), -1) == -1)
		{
			if (errno == EINTR)
				continue ;
			else
				throw std::runtime_error("SERVER: Poll failure");
		}
		checkEvents();
	}
}

void	Server::checkEvents(void)
{
	for (size_t i = 0; i < _listFd.size(); i++) {
		if (_listFd[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
			deleteClient(i, _listFd[i].fd);
			continue ;
		}
		if (_listFd[i].revents & POLLIN)
		{
			if (_listFd[i].fd == _fd)
				setNewClient();
			else
				receiveCommunication(i, _listFd[i].fd);
		}
		if (i< _listFd.size() && (_listFd[i].revents & POLLOUT))
			sendPendingData(i, _listFd[i].fd);
	}
}

//Setters and Getters
void	Server::setNewClient(void)
{
	Client	newClient;
	int		fd;
	
	fd = accept(_fd, 0, 0);
	if (fd == -1)
	{
		std::cerr << "Couldn't accept client";
		return ;
	}
	newClient.setFd(fd);
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Client bind failure";
		close(fd);
		return ;
	}
	std::cout << BLUE "A new client just connected !" RESET << std::endl;
	newClient.setPollFd();
	_listFd.push_back(newClient.getPollFd());
	_clients[newClient.getFd()] = newClient;
}

Client*	Server::getClientByNick(const std::string& nick) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (nick == it->second.getNickname())
			return (&(it->second));
	}
	return (NULL);
}

void	Server::receiveCommunication(size_t i, int fd)
{
	int		bits;
	char	buffer[1024] = { 0 };
    
	bits = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bits > 0)
	{
		_clients[fd].addBuffer(buffer);
		if (_clients[fd].getBuffer().find_first_not_of("\r\n") != std::string::npos)
			parsingBuffer(fd);
		if (DEBUG)
		{
			std::cout << "Message from client: " << fd  << " " << _clients[fd].getBuffer();
			if (_clients[fd].getBuffer() == "EXIT\n")
			{
				deleteClient(i, fd);
				close(_fd);
				exit (1);//A voir pour le leaks etc
			}
		}
	}
	else if (bits == 0 || bits == -1)
		deleteClient(i, fd);
}

void	Server::deleteClient(size_t i, int fd)
{
	close(fd);
	_clients.erase(fd);
	_listFd.erase(_listFd.begin() + i);
}

void	Server::parsingBuffer(int fd)
{
	if (DEBUG)
		std::cout << _clients[fd].getBuffer() << std::endl;
}

//Command Map Functions
/*void	Server::initCommands(void) {
	_commandMap["PASS"] = &Server::handlePass;
	_commandMap["NICK"] = &Server::handleNick;
	_commandMap["USER"] = &Server::handleUser;
	_commandMap["PING"] = &Server::handlePing;
	_commandMap["QUIT"] = &Server::handleQuit;
	_commandMap["PRIVMSG"] = &Server::handlePrivmsg;
}*/

void	Server::executeCommand(Client& client, const std::string& rawline) {
	Message msg;
	std::string	input = rawline;
	msg.parsing(input);

	std::string	command = msg.getParser().command;
	if (command.empty())
		return ;

	std::map<std::string, CommandHandler>::iterator it = _commandMap.find(command);
	if (it != _commandMap.end())
		(this->*(it->second))(client, msg);
	else
		return(saveOrSend(client, ERR_UNKNOWNCOMMAND(command)));//No match found
}

//Message Transmission
void	Server::saveOrSend(Client& client, const std::string& data) {
	if (!client.isSendBufferEmpty()) {//when _sendBuffer not empty
		client.appendToSendBuffer(data);
		return (enablePollout(client.getFd()));
	}
	ssize_t bytesSent = send(client.getFd(), data.c_str(), data.size(), 0);
	if (bytesSent >= 0 && static_cast<size_t>(bytesSent) == data.size())
		return ;//Everything was sent
	if (bytesSent > 0)
		client.appendToSendBuffer(data.substr(bytesSent));//Partial sent
	else
		client.appendToSendBuffer(data);//Nothing was sent
	enablePollout(client.getFd());
}

void	Server::enablePollout(int clientFd) {
	for (size_t i = 0; i < _listFd.size(); ++i) {
		if (clientFd == _listFd[i].fd) {
			_listFd[i].events |= POLLOUT;
			return ;
		}
	}
}

void	Server::disablePollout(int clientFd) {
	for (size_t i = 0; i < _listFd.size(); ++i) {
		if (clientFd == _listFd[i].fd) {
			_listFd[i].events &= ~POLLOUT;
			return ;
		}
	}
}

void	Server::sendPendingData(size_t i, int fd) {
	Client &client = _clients[fd];
	std::string &buffer = client.getSendBuffer();

	if (!buffer.empty()) {
		ssize_t bytes = send(fd, buffer.c_str(), buffer.size(), 0);
		if (bytes > 0)
			buffer.erase(0, bytes);
		else if (bytes == -1) {
			deleteClient(i, fd);
			return ;
		}
	}
	if (buffer.empty())
		disablePollout(fd);
}

//PRIVMSG <receiver> :<text to be sent>
void	Server::handlePrivmsg(Client &client, const Message& msg) {
	if (1) //a modifier
		return (saveOrSend(client, ERR_NOTREGISTERED()));

	const	std::vector<std::string>& params = msg.getParser().params;
	if (params.empty() ||params[0].empty())//411
		return(saveOrSend(client, ERR_NORECIPIENT(msg.getParser().command)));
	if (params.size() < 2 || params[1].empty())//412
		return (saveOrSend(client, ERR_NOTEXTTOSEND()));
	
	const std::string& target = params[0];
	const std::string& textToSend = params[1];
	if (!getClientByNick(target))//401
		return (saveOrSend(client, ERR_NOSUCHNICK(target)));
	//:<sender_nick>!<sender_user>@<sender_host>: PRIVMSG :<msg>\r\n 
	
	std::string prefix = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string line = ":" + prefix + " PRIVMSG " + target + " :" + textToSend + "\r\n";
	saveOrSend(*getClientByNick(target), line);
}
