/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:37:38 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/17 16:52:04 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/class/Channel.hpp"
#include "../../../includes/utils/Replies.hpp"
#include "../../../includes/utils/Colors.hpp"
#include <iostream>

void	Server::handleJoin(Client &client, const Message& msg)
{
	Parser	line = msg.getParser();
	std::string	channelName = parsingChannelName(client, line);
	if ((channelName.empty()))
		return;
	
	if (client.getNumberChannels() >= 10)
		saveOrSend(client, ERR_TOOMANYCHANNELS(channelName));
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
		return (joinChannel(client, it->second, line));
	createChannel(client, channelName);
}

std::string	Server::parsingChannelName(Client &client, const Parser &line)
{
	if (line.params.size() == 0)
	{
		std::string command = "JOIN";
		saveOrSend(client, ERR_NEEDMOREPARAMS(command));
		return "";
	}
	std::string	ChannelName = *(line.params.begin());

	if ((ChannelName[0] != '&' && ChannelName[0] != '#') || ChannelName.size() > 200
		|| ChannelName.size() < 2 || ChannelName.find_first_of("\a, ") != std::string::npos)
	{
		saveOrSend(client, ERR_NOSUCHCHANNEL(ChannelName));
		return "";
	}
	return (ChannelName);
}

void	Server::createChannel(Client &client, std::string channelName)
{
	Channel	newChannel(channelName);

	newChannel.addMember(client);
	newChannel.setOperator(client);
	_channels[channelName] = newChannel;
	saveOrSend(client, NEWMEMBERMESSAGE(client.getNickname()));
	if (DEBUG)
	{
		std::cout << "current channels :" << std::endl;
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			std::cout <<it->second.getName() << std::endl;
	}
}

void	Server::joinChannel(Client &client, Channel &channel, const Parser &line)
{
	try
	{
		joinVerification(client, channel, line);
		channel.addMember(client);
		/*std::string message = NEWMEMBERMESSAGE(client.getNickname());
    		send(client.getFd(), message.c_str(), message.size(), 0);*/
		return (channel.broadcast(*this, NEWMEMBERMESSAGE(client.getNickname())));
	}
	catch (int e)
	{
		return ;
	}
}

void	Server::joinVerification(Client &client, Channel &channel, const Parser &line)
{
	if (channel.isMember(client))
	{
		throw -1;
	}
	if (channel.isModeActive(I)) // check if channel is in invite only
	{
		saveOrSend(client, ERR_INVITEONLYCHAN(channel.getName()));
		throw -1;
	}
	if (channel.isModeActive(K) && (!line.params.size() <= 1 || !channel.isPwdCorrect(line.params[1])))
	{
		saveOrSend(client, ERR_BADCHANNELKEY(channel.getName()));
		throw -1;
	}
	if (channel.isModeActive(L) && channel.isMembersLimitReached()) // check if channel got to its limit
	{
		saveOrSend(client, ERR_CHANNELISFULL(channel.getName()));
		throw -1;
	}
}
