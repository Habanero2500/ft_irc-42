/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 20:04:25 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"
#include "../../includes/utils/Replies.hpp"
#include "../../includes/utils/Colors.hpp"

#include <iostream>
#include <unistd.h>

// **** ORTHODOX CANONICAL FORM ****
Server::Server()
{
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("SERVER: Socket failure");
	initCommands();
}

Server::Server(const Server &copy) : _fd(copy._fd){}//A regler

Server	&Server::operator=(const Server &copy)//A regler
{
    if(this != &copy)
    {
        _fd = copy._fd;
	}
    return *this;
}

Server::~Server()
{
	if (DEBUG)
	{
		std::cout << GREEN "\nServer destructor called" RESET << std::endl;
	}
	close(_fd);
}
