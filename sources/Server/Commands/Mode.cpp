/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:38:10 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 19:09:05 by malaimo          ###   ########.fr       */
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

	if (line.params.size() == 0)
	{
		std::string command = "MODE";
		saveOrSend(client, ERR_NEEDMOREPARAMS(client, command));
		return ;
	}
	if ((line.params[0][0] == '&' || line.params[0][0] == '#'))
		channelMode(client, line);
	else if (client.getNickname().compare(line.params[0]))
		saveOrSend(client, ERR_NOSUCHCHANNEL(client, line.params[0]));
}

void	Server::channelMode(Client &client, const Parser &line)
{
	std::map<std::string, Channel>::iterator it = _channels.find(line.params[0]);
	if (it == _channels.end())
		saveOrSend(client, ERR_NOSUCHCHANNEL(client, line.params[0]));
	else if (line.params.size() == 1)
		printChannelModes(client, it->second, line.params[0]);
	else if (!it->second.isMember(client))
		saveOrSend(client, ERR_NOTONCHANNEL(client, line.params[0]));	
	else if (!it->second.isClientAnOperator(client))
		saveOrSend(client, ERR_CHANOPRIVSNEEDED(client, line.params[0]));
	else
		manageChannelModes(client, it->second, line);
}

void	Server::printChannelModes(Client &client, Channel &channel, const std::string &channelName)
{
	std::string modeParams = "";
	if (channel.isModeActive(L))
		modeParams.append(channel.size_tToString(channel.getMaxMember()) + " ");
	if (channel.isMember(client) && channel.isModeActive(K))
		modeParams.append(channel.getPwd());
	saveOrSend(client, RPL_CHANNELMODEIS(client, channelName, channel.getModes(), modeParams));
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
			saveOrSend(client, ERR_UNKNOWNMODE(client, line.params[1][i]));
		else if (line.params[1][i] == '+')
			add = true;
		else if (line.params[1][i] == '-')
			add = false;
		else if (add)
			addMode(client, channel, line.params[1][i], it, end);
		else
			removeMode(client, channel, line.params[1][i], it, end);
	}
	// channel.broadcast(*this, MODESMODIFICATIONMESSAGE(client, line.params[0], channel.getModes(), channel.getModesArg()));
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
				saveOrSend(client, ERR_KEYSET(client, channel.getName()));
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
				saveOrSend(client, ERR_NOSUCHNICK(client, *it));
			else if (channel.setOperator(*targetClient))
				saveOrSend(client, ERR_USERNOTINCHANNEL(client, targetClient->getNickname(), channel.getName()));
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
			if (channel.isModeActive(K) && channel.isPwdCorrect(*it))
				channel.removemode(K);
			it++;
			break;
		case	'o':
		{
			if (it == end)
				break;
			Client* targetClient = getClientByNick(*it);
			if (targetClient == NULL)
				saveOrSend(client, ERR_NOSUCHNICK(client, *it));
			else if (channel.removeOperator(*targetClient))
				saveOrSend(client, ERR_USERNOTINCHANNEL(client, targetClient->getNickname(), channel.getName()));
			it++;
			break;
		}
		case	'l':
			channel.removemode(L);
			break;
	}
	
}

