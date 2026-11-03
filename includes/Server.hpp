/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/02 16:49:02 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "utils.hpp"

extern bool DEBUG;

class	Server
{
	private:
	int							_fd;
	struct sockaddr_in 			_adress;
	std::map<int, Client>		_clients;
	std::vector<struct pollfd>	_listFd;
	
	public:
	Server();
	Server(const Server &copy);
	Server	&operator=(const Server &copy);
	~Server();

	void	initServerAdress(int port);
	void	pollLoop();
	void	checkEvents(size_t *i);
	void	setNewClient();
	void	receiveCommunication(size_t *i ,int fd);
	void	deleteClient(size_t *i, int fd);
	void	parsingBuffer(int fd);
	void	executeCommand(int fd, const std::string &line);

	
};
#endif