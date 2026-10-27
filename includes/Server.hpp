/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/10/27 16:12:56 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

using namespace std;
class	Server
{
	private:
	int						_fd;
	struct sockaddr_in 		_adress;
	map<int, Client>		_clients;
	vector<struct pollfd>	_listFd;
	
	public:
	Server();
	Server(const Server &copy);
	Server	&operator=(const Server &copy);
	~Server();

	void	initServerAdress(int port);
	void	pollLoop();
	void	checkEvents(size_t *i);
	void	setNewClient();
	void	receiveCommunication(size_t *i);
	void	deleteClient(size_t *i);
	
};
#endif