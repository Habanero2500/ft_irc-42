/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SettersGetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:26:06 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/24 14:24:44 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"
#include "../../includes/class/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

void	Server::setNewClient(void) {
	int		fd;
	
	fd = accept(_fd, 0, 0);
	if (fd == -1)
	{
		std::cerr << "Couldn't accept client";
		return ;
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Client bind failure";
		close(fd);
		return ;
	}
	Client	&newClient = _clients[fd];
	newClient.setFd(fd);
	std::cout << BLUE "A new client just connected !" RESET << std::endl;
	newClient.setPollFd();
	_listFd.push_back(newClient.getPollFd());
}

Client*	Server::getClientByNick(const std::string& nick) 
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second.getNickname() == nick)
			return (&(it->second));
	}
	return (NULL);
}
