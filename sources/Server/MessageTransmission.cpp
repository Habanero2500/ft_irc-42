/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageTransmission.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:35:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/11 18:36:08 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"
#include "../../includes/class/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include <iostream>

void Server::isRegistered(Client &client)
{
	if(client.getNick() == true && client.getUser() == true && client.getPass() == true)
	{
		client.setRegistered();
		std::cout << BLUE "A new client just registered !" RESET << std::endl;
	}
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

void	Server::sendPendingData( int fd) {
	Client &client = _clients[fd];
	std::string &buffer = client.getSendBuffer();

	if (!buffer.empty()) {
		ssize_t bytes = send(fd, buffer.c_str(), buffer.size(), 0);
		if (bytes > 0)
			buffer.erase(0, bytes);
		else if (bytes == -1) {
			deleteClient(fd);
			return ;
		}
	}
	if (buffer.empty())
		disablePollout(fd);
}
