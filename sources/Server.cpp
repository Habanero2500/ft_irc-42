/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/10/27 16:17:46 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// **** ORTHODOX CANONICAL FORM ****

Server::Server()
{
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("SERVER: Socket failure");
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



//********** MAIN FUNCTIONS ************

void	Server::initServerAdress(int port)
{
	int	opt = 1;

	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(port);

	if (bind(_fd, (const sockaddr *)&_adress, sizeof(_adress)) == -1)
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
		for(size_t	i = 0; i < _listFd.size(); i++)
		{
			checkEvents(&i);
		}
	}
}

void	Server::checkEvents(size_t *i)
{
	if (_listFd[*i].revents & (POLLERR | POLLHUP | POLLNVAL))
	{
		deleteClient(i);
		return ;
	}
	if (_listFd[*i].revents & POLLIN)
	{
		if (_listFd[*i].fd == _fd)
			setNewClient();
		else
			receiveCommunication(i);
	}
}

void	Server::setNewClient()
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

void	Server::receiveCommunication(size_t *i)
{
	int		bits;
	char	buffer[1024] = { 0 };
    
	bits = recv(_listFd[*i].fd, buffer, sizeof(buffer) - 1, 0);
	if (bits > 0)
	{
		_clients[_listFd[*i].fd].addBuffer(buffer);
		cout << "Message from client: " << _listFd[*i].fd  << " " << buffer;
	}
	else if (bits == 0 || (bits == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)))
		deleteClient(i);
}

void	Server::deleteClient(size_t *i)
{
	int	fd = _listFd[*i].fd;
	close(fd);
	_clients.erase(fd);
	_listFd.erase(_listFd.begin() + *i);
	(*i)--;
}
