/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:47:55 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/06 16:00:39 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/IRC.hpp"
#include "../includes/class/Server.hpp"
#include "../includes/class/Client.hpp"
#include <stdlib.h>
#include <iostream>

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
	    server.initServerAdress(irc.getPort(), irc.getPwd());
	    server.pollLoop();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0) ; 
}
