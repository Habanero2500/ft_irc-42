/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:33:18 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/25 11:41:12 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/utils/Replies.hpp"
#include <iostream>

void	Server::handlePass(Client &client, const Message& msg)
{
	const Parser& pars = msg.getParser();
  
	if (pars.params.empty())
        return (saveOrSend(client, ERR_NEEDMOREPARAMS(client, pars.command)));

	if(client.getPass() == true) //pwd deja rentre
  	{
		if(DEBUG)
			std::cout << "Already registered" << std::endl;
    	return (saveOrSend(client, ERR_ALREADYREGISTRED(client)));
    }
	if(pars.params[0] != _pwd)
	{
		if(DEBUG)
			std::cout << "Wrong password" << std::endl;
		client.setToDisconnect();
		return (saveOrSend(client, ERR_PASSWDMISMATCH(client)));	
	}
	if(pars.params.size() < 1) // pas d'arg
	{
		if(DEBUG)
			std::cout << "Argument missing" << std::endl; 
		return(saveOrSend(client, ERR_NEEDMOREPARAMS(client, pars.command)));
	}
	else if(pars.params[0] == _pwd)
	{
		if(DEBUG)
			std::cout << "PWD OK" << std::endl;
		client.setPass();
		isRegistered(client);
		return;	
	}
}
