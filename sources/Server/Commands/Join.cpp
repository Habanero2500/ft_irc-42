/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:37:38 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 14:18:30 by malaimo          ###   ########.fr       */
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
	{
		std::string message = ERR_TOOMANYCHANNELS(channelName);
        	send(client.getFd(), message.c_str(), message.size(), 0);
	}
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
		return (joinChannel(client, it->second));
	createChannel(client, channelName);
}

std::string	Server::parsingChannelName(Client &client, const Parser &line)
{
	if (line.params.size() == 0)
	{
		std::string command = "JOIN";
		std::string message = ERR_NEEDMOREPARAMS(command);
        	send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	std::string	ChannelName = *(line.params.begin());

	if ((ChannelName[0] != '&' && ChannelName[0] != '#') || ChannelName.size() > 200
		|| ChannelName.size() < 2 || ChannelName.find_first_of("\a, ") != std::string::npos)
	{
		std::string message = ERR_NOSUCHCHANNEL(ChannelName);
        	send(client.getFd(), message.c_str(), message.size(), 0);
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
	std::string message = NEWMEMBERMESSAGE(client.getNickname());
    send(client.getFd(), message.c_str(), message.size(), 0);
	if (DEBUG)
	{
		std::cout << "current channels :" << std::endl;
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			std::cout <<it->second.getName() << std::endl;
	}
}

void	Server::joinChannel(Client &client, Channel &channel)
{
	try
	{
		joinVerification(client, channel);
		channel.addMember(client);
		std::string message = NEWMEMBERMESSAGE(client.getNickname());
    	send(client.getFd(), message.c_str(), message.size(), 0);
		if (DEBUG)
		{
			std::cout << BLUE "channel: " << channel.getName() << " accepted new member: " << client.getNickname() << RESET << std::endl;
			// std::cout << "current members :" << std::endl;
			// for (size_t i = 0; i < channel.getNumberMember(); i++)
			// {
			// 	std::cout << channel.getMember().client.getNickname() << std::endl;
			// }
		}
	}
	catch (int e)
	{
		return ;
	}
}

void	Server::joinVerification(Client &client, Channel &channel)
{
	if (channel.isMember(client))
	{
		throw -1;
	}
	if (channel.isModeActive(I)) // check if channel is in invite only
	{
		std::string message = ERR_INVITEONLYCHAN(channel.getName());
        	send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
	if (channel.isModeActive(K)) // check du mot de pass du channel a rajouter
	{
		std::string message = ERR_BADCHANNELKEY(channel.getName());
        	send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
	if (channel.isModeActive(L) && channel.isMembersLimitReached()) // check if channel got to its limit
	{
		std::string message = ERR_CHANNELISFULL(channel.getName());
        	send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
	if (channel.isMemberBanned(client))
	{
		std::string message = ERR_BANNEDFROMCHAN(channel.getName());
        	send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
}
