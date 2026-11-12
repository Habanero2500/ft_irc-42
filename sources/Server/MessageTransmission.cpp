/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageTransmission.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:35:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 20:24:35 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"

void Server::isRegistered(Client &client)
{
	if(client.getNick() == true && client.getUser() == true && client.getPass() == true)
	{
		client.setRegistered();
		if(DEBUG)
			std::cout << BLUE "A new client just registered !" RESET << std::endl;
		saveOrSend(client, ":localhost 001 " + client.getNickname() + " :Welcome to the IRC Network, "
			+ client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + "\r\n");
		saveOrSend(client, ":localhost 002 " + client.getNickname() + " :Your host is localhost, running version 1.0\r\n");
		saveOrSend(client, ":localhost 003 " + client.getNickname() + " :This server was created today\r\n");
		saveOrSend(client, ":localhost 004 " + client.getNickname() + " localhost 1.0 o itkol\r\n");
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