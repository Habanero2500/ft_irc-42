/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/10/26 18:00:07 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server()
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
}

Server::Server(const Server &copy) : _fd(copy._fd){}

Server	&Server::operator=(const Server &copy)
{
    if(this != &copy)
    {
        _fd = copy._fd;
    }
    return *this;
}


Server::~Server()
{
	close(_fd);
}


//======================================================================


void	Server::initServerAdress(int port)
{
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(port);

	bind(_fd, (const sockaddr *)&_adress, sizeof(_adress));
	listen(_fd, 128);
	
	struct pollfd	serverPollFd;
	serverPollFd.fd = _fd;
	serverPollFd.events = POLLIN;
	serverPollFd.revents = 0;
	
	_listFd.push_back(serverPollFd);
}

void	Server::receiveData()
{
	int clientSocket = accept(_fd, 0, 0);

    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;
}

void	Server::pollLoop()
{
	poll(&_listFd[0], _listFd.size(), -1);
	for(size_t	i = 0; i < _listFd.size(); i++)
	{
		if (_listFd[i].revents & POLLIN)
		{
			if (_listFd[i].fd == _fd)
			{
				// Client	*newClient = new Client();
				// _clients[i + 1] = *newClient;
				accept(_fd, 0, 0);
				
			}
		}
	}
}