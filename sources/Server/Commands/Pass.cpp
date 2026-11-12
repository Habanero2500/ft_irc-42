/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:33:18 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 22:18:13 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"

void	Server::handlePass(Client &client, const Message& msg)
{
	Parser pars = msg.getParser();
  
	if(client.getAttempt() > 2)
	{
		if(DEBUG)
			std::cout << "Too much attempts" << std::endl;
		deleteClient(client.getFd());
		return;
	}
	if (pars.params.empty())
        return(saveOrSend(client, ERR_NEEDMOREPARAMS(pars.command)));

	if(client.getPass() == true) //pwd deja rentre
  	{
		if(DEBUG)
			std::cout << "Already registered" << std::endl;
    	return(saveOrSend(client, ERR_ALREADYREGISTRED()));
    }
	if(pars.params[0] != _pwd)
	{
		if(DEBUG)
			std::cout << "Wrong password" << std::endl;
		client.increaseAttempt();
		return(saveOrSend(client, ERR_PASSWDMISMATCH()));	
	}
	if(pars.params.size() < 1) // pas d'arg
	{
		if(DEBUG)
			std::cout << "Argument missing" << std::endl; 
		return(saveOrSend(client, ERR_NEEDMOREPARAMS(pars.command)));
	}
	else if(pars.params[0] == _pwd)
	{
		if(DEBUG)
			std::cout << "PWD OK" << std::endl;
		client.setPass();
		return;	
	}
}