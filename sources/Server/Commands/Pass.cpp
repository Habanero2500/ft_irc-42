/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:33:18 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 20:38:17 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"

void	Server::handlePass(Client &client, const Message& msg)
{
	Parser pars = msg.getParser();  
	if(client.getAttempt() > 2) //Trop de tentatives
	{
		if(DEBUG)
			std::cout << "Too much attempts" << std::endl;
		deleteClient(client.getFd());
		return;
	}
	if (pars.params.empty())
    {
        std::string message = ERR_NEEDMOREPARAMS(pars.command);
        send(client.getFd(), message.c_str(), message.size(), 0);
        return;
    }
	if(client.getPass() == true) //pwd deja rentre
  	{
		if(DEBUG)
			std::cout << "Already registered" << std::endl;
        std::string message = ERR_ALREADYREGISTRED();
        send(client.getFd(), message.c_str(), message.size(), 0);
    	return;
    }
	if(pars.params[0] != _pwd)
	{
		if(DEBUG)
			std::cout << "Wrong password" << std::endl;
		std::string message = ERR_PASSWDMISMATCH();
        send(client.getFd(), message.c_str(), message.size(), 0);
		client.increaseAttempt();
		return;	
	}
	if(pars.params.size() < 1) // pas d'arg
	{
		if(DEBUG)
			std::cout << "Argument missing" << std::endl; 
		std::string msg = "PASS";
		std::string message = ERR_NEEDMOREPARAMS(msg);
        send(client.getFd(), message.c_str(), message.size(), 0);
		return;
	}
	else if(pars.params[0] == _pwd)
	{
		if(DEBUG)
			std::cout << "PWD OK" << std::endl;
		client.setPass();
		return;	
	}
}