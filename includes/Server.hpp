/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/10/26 17:57:04 by malaimo          ###   ########.fr       */
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
	map<size_t, Client>		_clients;
	vector<struct pollfd>	_listFd;
	
	public:
	Server();
	Server(const Server &copy);
	Server	&operator=(const Server &copy);
	~Server();

	void	initServerAdress(int port);
	void	receiveData();
	void	pollLoop();
	
};
#endif