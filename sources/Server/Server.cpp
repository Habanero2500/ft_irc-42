/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/11 18:38:32 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"

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
	std::cout << "destructor called" << std::endl;
	close(_fd);
}

