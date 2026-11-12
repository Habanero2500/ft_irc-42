/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:34:01 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 14:10:55 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/utils/Replies.hpp"

void	Server::handleNick(Client &client, const Message& msg)
{
	Parser	line = msg.getParser();
	
	std::string	Nickname = parsingNick(client, line);
	if ((Nickname.empty()))
		return;

	client.setNickname(Nickname);
	client.setNick();
	if (DEBUG)
	{
		std::string message = "Nick ok\r\n";
    	send(client.getFd(), message.c_str(), message.size(), 0);
	}
	isRegistered(client);
		
}

std::string	Server::parsingNick(Client &client, const Parser &line)
{
	if (line.params.size() == 0)
	{
		std::string message = ERR_NONICKNAMEGIVEN();
        send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	
	
	std::string	NicknameGiven = *(line.params.begin());

	if (NicknameGiven.find_first_not_of(std::string(LETTERS) + NUMBERS + SPECIAL_IRC) != std::string::npos 
		|| NicknameGiven.size() > 9 || isdigit(NicknameGiven[0]) || NicknameGiven[0] == '-')
	{
		std::string message = ERR_ERRONEUSNICKNAME(NicknameGiven);
        send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first == client.getFd()) // On compare pas avec le premier 
            continue;
		if (it->second.getNickname() == NicknameGiven)
		{
			std::string message = ERR_NICKNAMEINUSE(NicknameGiven);
        	send(client.getFd(), message.c_str(), message.size(), 0);
			return "";
		}
	}
	return (NicknameGiven);
}
