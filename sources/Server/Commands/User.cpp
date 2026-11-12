/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:34:55 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 22:24:12 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"

static bool isValidUsername(const std::string &s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c <= ' ' || c == 127 || c == '@' || c == '!')
			return false;
	}
	return true;
}

static bool isValidRealname(const std::string &s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c < ' ' || c == 127)
			return false;
	}
	return true;
}

void	Server::handleUser(Client &client, const Message& msg)
{
	if(client.getPass() != true) 
	{
		if(DEBUG)
			std::cout << "Password has to be set before anything" << std::endl;
		return;
	}
	
	Parser pars = msg.getParser();
	
	if (client.getUser() == true) 
		return(saveOrSend(client, ERR_ALREADYREGISTRED()));
	if (pars.params.size() < 4) 
		return(saveOrSend(client, ERR_NEEDMOREPARAMS(pars.command)));
	
	std::string			username = pars.params[0];    // USER <username> <mode> <unused> :<realname>
	const std::string	&realname = pars.params[3];              
	
	if (!isValidUsername(username) || !isValidRealname(realname))
	{
		if (DEBUG)
			std::cout << "Wrong argument --> " << username << " / " << realname << std::endl;
		return ;
	}
	client.setUsername(username);
	client.setRealName(realname);
	client.setHostname(pars.params[1]);
	client.setServername(pars.params[2]);
	isRegistered(client);
}
