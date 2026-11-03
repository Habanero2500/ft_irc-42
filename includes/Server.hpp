/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/03 16:32:21 by jmetayer         ###   ########.fr       */
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
	std::string					_pwd;
	int							_port;
	
	public:
	Server();
	Server(const Server &copy);
	Server	&operator=(const Server &copy);
	~Server();

	void	initServerAdress(int port, std::string pwd);
	void	pollLoop();
	void	checkEvents(size_t *i);
	void	setNewClient();
	void	receiveCommunication(size_t *i ,int fd);
	void	deleteClient(size_t *i, int fd);
	void	parsingBuffer(int fd);
	void	executeCommand(int fd, const std::string &line);

	
};
#endif