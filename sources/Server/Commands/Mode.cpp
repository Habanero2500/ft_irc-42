/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:38:10 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/11 18:38:24 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"

void	Server::handleMode(Client &client, const Message& msg)
{
	Parser	line = msg.getParser();
	
	if (line.params.size() == 0)
	{
		std::string command = "MODE";
		saveOrSend(client, ERR_NEEDMOREPARAMS(command));
		return ;
	}
	if (line.params[0][0] == '&' || line.params[0][0] == '#')
		channelMode(client, line);
	else
		userMode(client, line);
}

void	Server::channelMode(Client &client, const Parser &line)
{
	std::map<std::string, Channel>::iterator it = _channels.find(line.params[0]);
	if (it == _channels.end())
		saveOrSend(client, ERR_NOSUCHCHANNEL(line.params[0]));
	else
	{
		if (line.params.size() == 1)
		{
			printChannelModes(client, it, line.params[0]);
		}
		else
		{
			parsing
		}
	}
}

void	Server::printChannelModes(Client &client, const std::map<std::string, Channel>::iterator &it, const std::string &channel)
{
	std::string mode = it->second.getModes();
	std::string modeParams = "";
	if (it->second.isModeActive(L))
		modeParams.append(size_tToString(it->second.getMaxMember()));
	if (it->second.isModeActive(K))
		modeParams.append(it->second.getPwd());
	saveOrSend(client, RPL_CHANNELMODEIS(channel, mode, modeParams));
}


void	Server::userMode(Client &client, const Parser &line)
{
	
}
