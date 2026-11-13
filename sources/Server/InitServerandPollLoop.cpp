/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitServerandPollLoop.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:18:09 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 17:21:57 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"
#include "../../includes/class/Client.hpp"
#include "../../includes/class/Message.hpp"
#include "../../includes/utils/Replies.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

void	Server::initServerAdress(int port, std::string pwd)
{
	int	opt = 1;

	std::cout << WELCOME_MESSAGE << std::endl;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);

	_pwd = pwd;

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

void	Server::checkEvents()
{
	for(size_t	i = 0; i < _listFd.size(); i++)
	{
		if (_listFd[i].revents & (POLLERR | POLLHUP | POLLNVAL))
		{
			deleteClient(_listFd[i].fd);
			return ;
		}
		if (_listFd[i].revents & POLLIN)
		{
			if (_listFd[i].fd == _fd)
				setNewClient();
			else
				receiveCommunication(_listFd[i].fd);
		}
		if (i< _listFd.size() && (_listFd[i].revents & POLLOUT))
			sendPendingData(_listFd[i].fd);
	}
}

void	Server::receiveCommunication(int fd)
{
	int		bits;
	char	buffer[1024] = { 0 };
    
	bits = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bits > 0)
	{
		_clients[fd].addBuffer(buffer);
		if (DEBUG)
		{
			std::cout << "Message from client: " << fd  << " " << _clients[fd].getBuffer();
			if (_clients[fd].getBuffer() == "EXIT\n")
			{
				deleteClient(fd);
				close(_fd);
				exit (1);//A voir pour le leaks etc
			}
		}
		parsingBuffer(fd);
		if (_clients.find(fd) != _clients.end() && _clients[fd].shouldDisconnect())
			deleteClient(fd);
	}
}

void	Server::deleteClient(int fd)
{
	close(fd);
	_clients.erase(fd);
	for (std::vector<struct pollfd>::iterator it = _listFd.begin(); it != _listFd.end(); it++)
	{
		if (it->fd == fd)
		{
			_listFd.erase(it);
			return ;
		}
	}
}

void	Server::parsingBuffer(int fd)
{
	Client		&client = _clients[fd];
	std::string	buf = client.getBuffer();
	size_t		pos;

	while ((pos = buf.find('\n')) != std::string::npos)
	{
		std::string line = buf.substr(0, pos);
		buf.erase(0, pos + 1);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (!line.empty())
			executeCommand(client, line);
		if (client.shouldDisconnect())
			break ;
	}
	client.setBuffer(buf);
}

// Command Map Functions
void	Server::initCommands(void) {
	_commandMap["PASS"] = &Server::handlePass;
	_commandMap["NICK"] = &Server::handleNick;
	_commandMap["USER"] = &Server::handleUser;
	// _commandMap["PING"] = &Server::handlePing;
	_commandMap["QUIT"] = &Server::handleQuit;
	_commandMap["PRIVMSG"] = &Server::handlePrivmsg;
	_commandMap["JOIN"] = &Server::handleJoin;
	if(DEBUG)
		_commandMap["DISPLAY"] = &Server::handleDisplay;
}

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
