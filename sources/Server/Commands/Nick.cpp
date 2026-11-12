/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:34:01 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 22:13:16 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"

int	Server::parsingNick(Client &client, const Parser &line)
{
	if (line.params.size() == 0)
		return (1);
	
	std::string	NicknameGiven = *(line.params.begin());

	if (NicknameGiven.find_first_not_of(std::string(LETTERS) + NUMBERS + SPECIAL_IRC) != std::string::npos 
		|| NicknameGiven.size() > 9 || isdigit(NicknameGiven[0]) || NicknameGiven[0] == '-')
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
	Parser	line = msg.getParser();

	switch(parsingNick(client, line))
	{
		case 0 : 
			break;
		case 1 :
			return(saveOrSend(client, ERR_NONICKNAMEGIVEN()));
		case 2 : 
			return(saveOrSend(client, ERR_ERRONEUSNICKNAME(*(line.params.begin()))));
		case 3 :
			return(saveOrSend(client, ERR_NICKNAMEINUSE(line.params[0])));
	}

	if ((line.params[0].empty()))
		return;

	client.setNickname(*(line.params.begin()));
	client.setNick();
	if (DEBUG)
	{
		std::string message = "Nick ok\r\n";
    	send(client.getFd(), message.c_str(), message.size(), 0);
	}
	isRegistered(client);
}

