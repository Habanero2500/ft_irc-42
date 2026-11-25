/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:34:01 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/25 13:26:14 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/utils/Replies.hpp"
#include <iostream>

int	Server::parsingNick(Client &client, const Parser &line)
{
	if (!client.getPass())
	{
		if (DEBUG)
			std::cout << RED "no password" RESET << std::endl;
		return (4);
	}
	if (!line.params.size())
		return (1);
	
	const std::string&	NicknameGiven = *(line.params.begin());

	if (NicknameGiven.find_first_not_of(std::string(LETTERS) + NUMBERS + SPECIAL_IRC) != std::string::npos 
		|| NicknameGiven.size() > 9 || NicknameGiven.size() < 1 || !isalpha(NicknameGiven[0]))
		return (2);

	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first == client.getFd()) // On compare pas avec le premier 
            continue;
		if (it->second.getNickname() == NicknameGiven)
			return (3);
	}
	return (0);
}

void	Server::handleNick(Client &client, const Message& msg)
{
	const Parser&	line = msg.getParser();

	switch(parsingNick(client, line))
	{
		case 0 : 
			break;
		case 1 :
			return(saveOrSend(client, ERR_NONICKNAMEGIVEN(client)));
		case 2 : 
			return(saveOrSend(client, ERR_ERRONEUSNICKNAME(client, *(line.params.begin()))));
		case 3 :
			return(saveOrSend(client, ERR_NICKNAMEINUSE(client, line.params[0])));
		case 4 :
			return;
	}

	if ((line.params[0].empty()))
		return;

	if (!client.isRegistered())
		client.setNickname(*(line.params.begin()));
	else
	{
		std::string prefix = client.changeNickname(*(line.params.begin()));
		saveOrSend(client, NEWNICKMESSAGE(prefix, client));
	}
	client.setNick();
	// client.setNickname(*(line.params.begin()));
	if (DEBUG)
		std::cout << "Nick ok" << std::endl;
	isRegistered(client);
}

