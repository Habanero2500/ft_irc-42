/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:47:55 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 16:44:57 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

int main(int argc, char **argv)
{
    // if(argc != 3)
    // {
    //     std::cerr << "This program has to take two arguments such as : './ircserv <port> <password>'" << std::endl;
    //     return(1);
    // }
    // try
    // {
    //     irc serv(argv[1], argv[2]);
    // }
    // catch(std::exception& e)
    // {
    //     std::cerr << e.what() << std::endl;
    //     return (1);
    // }

	(void)argc;
	(void)argv;
	
	Server	server;
	
	server.initServerAdress(6667);
	server.receiveData();
	
    return (0) ; 
}