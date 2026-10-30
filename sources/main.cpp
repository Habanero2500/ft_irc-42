/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:47:55 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/30 15:13:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include <stdlib.h>

#ifdef DEBUG_MODE
    bool DEBUG = true;
#else
    bool DEBUG = false;
#endif

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        std::cerr << "This program has to take two arguments such as : './ircserv <port> <password>'" << std::endl;
        return(1);
    }
    try
    {
        irc irc(argv[1], argv[2]);
        Server	server;
	    server.initServerAdress(irc.getPort());
	    server.pollLoop();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0) ; 
}