/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:38:10 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/13 14:26:37 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"

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
	else if (!it->second.isMember(client))
		saveOrSend(client, ERR_NOTONCHANNEL(line.params[0]));
	else if (!it->second.isOperator(client))
		saveOrSend(client, ERR_CHANOPRIVSNEEDED(line.params[0]));
	else
	{
		if (line.params.size() == 1)
		{
			printChannelModes(client, it->second, line.params[0]);
		}  	
		else
		{
			manageChannelModes(client, it->second, line);
		}
	}
}

void	Server::printChannelModes(Client &client, Channel &channel, const std::string &channelName)
{
	std::string mode = channel.getModes();
	std::string modeParams = "";
	if (channel.isModeActive(L))
		modeParams.append(size_tToString(channel.getMaxMember()));
	if (channel.isModeActive(K))
		modeParams.append(channel.getPwd());
	saveOrSend(client, RPL_CHANNELMODEIS(channelName, mode, modeParams));
}

void	Server::manageChannelModes(Client &client, Channel &channel, const Parser &line)
{
	std::string	needParams = "lbvok";
	int			argument = 2;
	
	if (line.params[1].find_first_not_of("+-psitnmovblk") != std::string::npos)
		saveOrSend(client, ERR_CHANOPRIVSNEEDED(line.params[0]));
	bool	add = true;
	for(size_t i = 0; i < line.params[1].size(); i++)
	{
		if (line.params[1][i] == '+')
			add = true;
		else if (line.params[1][i] == '-')
			add = false;
		else if (add)
		{
			if (needParams.find(line.params[1][i]))
			{
				switch	(line.params[1][i])
				{
					case	'l':
						channel.setMaxMembers(line.params[argument++]);
						channel.setmode(static_cast<int>(line.params[1][i]));
						break;
					case	'b':
						channel.setMaxMembers(line.params[argument++]);
						channel.setmode(static_cast<int>(line.params[1][i]));
						
				}
			}
		}
			
	}
}

// void	Server::addChannelMode(Channel &channel, const Parser &line, char mode, )
// {
// 	std::string	needParams
// 	if (mode)
		
// 	channel.setmode(static_cast<int>(mode));
// }

// lbvok

void	Server::userMode(Client &client, const Parser &line)
{
	(void)client;
	(void)line;
}
