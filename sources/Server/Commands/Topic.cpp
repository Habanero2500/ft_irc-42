/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 20:28:40 by opapouts          #+#    #+#             */
/*   Updated: 2026/11/12 20:28:44 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Channel.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/utils/Replies.hpp"

//TOPIC <channel> [<topic>]
void	Server::handleTopic(Client& client, const Message& msg) {
	if (!topicValidation(client, msg))
		return ;

	const std::vector<std::string>& params = msg.getParser().params;
	const std::string& channelName = params[0];
	Channel& channel = _channels.find(channelName)->second;

	if (params.size() == 1) {//Display topic
		if (!channel.isTopicSet())
			return (saveOrSend(client, RPL_NOTOPIC(client, channelName)));//331

		else
			return (saveOrSend(client, RPL_TOPIC(client, channelName, channel.getTopic())));//332
	}
	else if (params.size() >= 2) {//Change topic
		if (channel.isModeActive(T) && !channel.isClientAnOperator(client))
			return (saveOrSend(client, ERR_CHANOPRIVSNEEDED(client, channelName)));//482
		channel.setTopic(params[1]);
		const std::string line = ":" + getPrefixForCommand(client) + " TOPIC " + channelName + " :" + params[1] + "\r\n";
		return (channel.broadcast(*this, line));//332
	}
}

//When we change the topic we broadcast it on everyone in the channel
bool	Server::topicValidation(Client& client, const Message& msg) {
	if (!client.isRegistered())//451
		return (saveOrSend(client, ERR_NOTREGISTERED(client)), false);
	
	const std::vector<std::string>& params = msg.getParser().params;
	if (params.size() < 1)
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(client, std::string("TOPIC"))), false);//461

	const std::string& channelName = params[0];
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it == _channels.end())
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(client, channelName)), false);//403

	if (!it->second.getMemberByNick(client.getNickname()))
		return (saveOrSend(client, ERR_NOTONCHANNEL(client, channelName)), false);//442
	return (true);
}
