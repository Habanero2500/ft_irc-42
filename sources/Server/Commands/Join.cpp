/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:37:38 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/24 14:29:49 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/class/Channel.hpp"
#include "../../../includes/utils/Replies.hpp"
#include <iostream>

void	Server::handleJoin(Client &client, const Message& msg)
{
	if(!client.isRegistered())
		return(saveOrSend(client, ERR_NOTREGISTERED(client)));
	const Parser&	line = msg.getParser();
	if (!line.params.size())
		return(saveOrSend(client, ERR_NEEDMOREPARAMS(client, line.command)));

	const std::vector<std::string>&	channels = splitComma(line.params[0]);
	std::vector<std::string>	pwd;
	
	if (line.params.size() > 1)
		pwd = splitComma(line.params[1]);
	for (size_t	i = 0; i < channels.size(); i++)
	{
		try 
		{
			parsingChannelName(client, channels[i]);
			if (client.getNumberChannels() >= 10)
			{
				saveOrSend(client, ERR_TOOMANYCHANNELS(client, channels[i]));
				throw -1;
			}
			std::map<std::string, Channel>::iterator itChannel = _channels.find(channels[i]);
			if (itChannel != _channels.end())
				joinChannel(client, itChannel->second, getArgument(pwd, i));
			else
				createChannel(client, channels[i]);
		}
		catch (int e)
		{
			continue ;
		}
	}
}

void	Server::parsingChannelName(Client &client, const std::string& channelName)
{
	if ((channelName[0] != '&' && channelName[0] != '#') || channelName.size() > 200
		|| channelName.size() < 2 || channelName.find_first_of("\a, ") != std::string::npos)
	{
		saveOrSend(client, ERR_NOSUCHCHANNEL(client, channelName));
		throw -1;
	}
}

void	Server::createChannel(Client &client, const std::string& channelName)
{
	Channel	newChannel(channelName);

	newChannel.addMember(client);
	newChannel.setOperator(client);
	_channels[channelName] = newChannel;
	announceJoin(client, newChannel);
	// saveOrSend(client, NEWMEMBERMESSAGE(client.getNickname()));
	if (DEBUG)
	{
		std::cout << "current channels :" << std::endl;
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			std::cout <<it->second.getName() << std::endl;
	}
}

void	Server::joinChannel(Client &client, Channel &channel, const std::string& pwd)
{
	try
	{
		joinVerification(client, channel, pwd);
		channel.addMember(client);
		/*std::string message = NEWMEMBERMESSAGE(client.getNickname());
    		send(client.getFd(), message.c_str(), message.size(), 0);*/
		//return (channel.broadcast(*this, NEWMEMBERMESSAGE(client.getNickname())));
		return(announceJoin(client, channel));
	}
	catch (int e)
	{
		return ;
	}
}

void	Server::joinVerification(Client &client, Channel &channel, const std::string& pwd) {
	if (channel.isMember(client))
		throw -1;
	if (channel.isModeActive(I)) // check if channel is in invite only
	{
		saveOrSend(client,ERR_INVITEONLYCHAN(client, channel.getName()));
		throw -1;
	}
	if (channel.isModeActive(K) && !(channel.isPwdCorrect(pwd)))
	{
		saveOrSend(client,ERR_BADCHANNELKEY(client, channel.getName()));
		throw -1;
	}
	if (channel.isModeActive(L) && channel.isMembersLimitReached()) // check if channel got to its limit
	{
		saveOrSend(client,ERR_CHANNELISFULL(client, channel.getName()));
		throw -1;
	}
}

void	Server::announceJoin(Client &client, Channel &channel) {
	std::string	name = channel.getName();
	channel.broadcast(*this, ":" + client.getPrefix() + " JOIN " + name + "\r\n"); // All the present member on the channel receuve this
	if (channel.getTopic().empty())// Topic is displayed, only for the new member
		saveOrSend(client, RPL_NOTOPIC(client, name));
	else
		saveOrSend(client, RPL_TOPIC(client, name, channel.getTopic()));
	saveOrSend(client, RPL_NAMREPLY(client, name, channel.getNamesList())); //All the members name are displayed
	saveOrSend(client, RPL_ENDOFNAMES(client, name));
}
