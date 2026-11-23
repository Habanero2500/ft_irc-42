/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SettersGetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:26:06 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/17 13:47:49 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"
#include "../../includes/class/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

void	Server::setNewClient(void) {
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
		if (it->second.getNickname() == nick)
			return (&(it->second));
	}
	return (NULL);
}

const std::string	Server::getPrefixForCommand(Client& client) const {
	const std::string prefix = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	return (prefix);
}

Channel*	Server::getChannelByNick(const std::string& name) {
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return (NULL);
	return (&(it->second));
}

std::string    Server::getArgument(std::vector<std::string>& array, size_t& index)
{
	if (index < array.size())
		return (array[index]);
	return ("");
}
