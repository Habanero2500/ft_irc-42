/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:38:10 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/17 15:07:09 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/utils/Replies.hpp"
#include "../../../includes/utils/Colors.hpp"
#include <iostream>

void	Server::handleMode(Client &client, const Message& msg)
{
	Parser	line = msg.getParser();

	if (DEBUG)
	{
		std::cout << YELLOW "HANDLING MODE" RESET << std::endl;
	}
	if (line.params.size() == 0)
	{
		std::string command = "MODE";
		saveOrSend(client, ERR_NEEDMOREPARAMS(command));
		return ;
	}
	if (line.params[0][0] == '&' || line.params[0][0] == '#')
		channelMode(client, line);
	else
		saveOrSend(client, ERR_NOSUCHSERVER(line.params[0]));
}

void	Server::channelMode(Client &client, const Parser &line)
{
	if (DEBUG)
	{
		std::cout << YELLOW "HANDLING CHANNEL MODE" RESET << std::endl;
	}
	std::map<std::string, Channel>::iterator it = _channels.find(line.params[0]);
	if (it == _channels.end())
		saveOrSend(client, ERR_NOSUCHCHANNEL(line.params[0]));
	else if (!it->second.isMember(client))
		saveOrSend(client, ERR_NOTONCHANNEL(line.params[0]));
	else if (!it->second.isClientAnOperator(client))
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
	if (DEBUG)
	{
		std::cout << YELLOW "PRINTING MODE" RESET << std::endl;
	}
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
	std::string allowedcharacters = "+-itkol";
	std::vector<std::string>::const_iterator	it = line.params.begin() + 2;
	std::vector<std::string>::const_iterator	end = line.params.end();
	bool	add = true;

	for(size_t i = 0; i < line.params[1].size(); i++)
	{
		if (allowedcharacters.find(line.params[1][i]) == std::string::npos)
			saveOrSend(client, ERR_UNKNOWNMODE(line.params[1][i]));
		else if (line.params[1][i] == '+')
			add = true;
		else if (line.params[1][i] == '-')
			add = false;
		else if (add)
			addMode(client, channel, line.params[1][i], it, end);
		else 
			removeMode(client, channel, line.params[1][i], it, end);	
	}
}


void Server::addMode(Client &client, Channel &channel, char mode, std::vector<std::string>::const_iterator &it, const std::vector<std::string>::const_iterator &end)
{
	switch	(mode)
	{
		case	'i':
			channel.setmode(I);
			break;
		case	't':
			channel.setmode(T);
			break;
		case	'k':
			if (it == end)
				break;
			if (channel.isModeActive(K))
				saveOrSend(client, ERR_KEYSET(channel.getName()));
			else
			{
				channel.setPwd(*it);
				channel.setmode(K);
			}
			it++;
			break;
		case	'o':
		{
			if (it == end)
				break;
			Client* targetClient = getClientByNick(*it);
			if (targetClient == NULL)
				saveOrSend(client, ERR_NOSUCHNICK(*it));
			else if (!channel.isClientAnOperator(*targetClient))
				saveOrSend(client, ERR_CHANOPRIVSNEEDED(channel.getName()));
			else if (channel.setOperator(*targetClient))
				saveOrSend(client, ERR_USERNOTINCHANNEL(targetClient->getNickname(), channel.getName()));
			it++;
			break;
		}
		case	'l':
			if (it == end)
				break;
			channel.setMaxMembers(*it);
			channel.setmode(L);
			it++;
			break;
	}
}

void Server::removeMode(Client &client, Channel &channel, char mode, std::vector<std::string>::const_iterator &it, const std::vector<std::string>::const_iterator &end)
{
	switch	(mode)
	{
		case	'i':
			channel.removemode(I);
			break;
		case	't':
			channel.removemode(T);
			break;
		case	'k':
			if (it == end)
				break;
			if (!channel.isModeActive(K))
				break;
			if (channel.isPwdCorrect(*it))
				channel.removemode(K);
			it++;
			break;
		case	'o':
		{
			if (it == end)
				break;
			Client* targetClient = getClientByNick(*it);
			if (targetClient == NULL)
				saveOrSend(client, ERR_NOSUCHNICK(*it));
			else if (!channel.isClientAnOperator(*targetClient))
				saveOrSend(client, ERR_CHANOPRIVSNEEDED(channel.getName()));
			else if (channel.removeOperator(*targetClient))
				saveOrSend(client, ERR_USERNOTINCHANNEL(targetClient->getNickname(), channel.getName()));
			it++;
			break;
		}
		case	'l':
			channel.removemode(L);
			break;
	}
	
}

