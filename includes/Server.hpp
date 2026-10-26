/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/10/26 14:44:16 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class	Server
{
	private:
	int						_fd;
	struct sockaddr_in 		_adress;
	
	public:
	Server();
	Server(const Server &copy);
	Server	&operator=(const Server &copy);
	~Server();

	void	initServerAdress(int port);
	void	receiveData();
	
};
#endif