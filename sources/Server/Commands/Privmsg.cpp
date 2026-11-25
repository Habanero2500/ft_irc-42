/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:36:32 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 19:39:25 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/utils/Replies.hpp"
#include "../../../includes/class/Channel.hpp"

//PRIVMSG <receiver> :<text to be sent>
void	Server::handlePrivmsg(Client &client, const Message& msg) {
	if (!client.isRegistered())
		return (saveOrSend(client, ERR_NOTREGISTERED(client)));

	const	std::vector<std::string>& params = msg.getParser().params;
	if (params.empty() || params[0].empty())//411
		return (saveOrSend(client, ERR_NORECIPIENT(client, msg.getParser().command)));
	if (params.size() < 2 || params[1].empty())//412
		return (saveOrSend(client, ERR_NOTEXTTOSEND(client)));
	
	const std::string& target = params[0];
	const std::string& textToSend = params[1];
	if (!getClientByNick(target) && !getChannelByNick(target))//401
		return (saveOrSend(client, ERR_NOSUCHNICK(client, target)));

	//:<sender_nick>!<sender_user>@<sender_host>: PRIVMSG :<msg>\r\n 
	const std::string prefix = getPrefixForCommand(client);
	const std::string line = ":" + prefix + " PRIVMSG " + target + " :" + textToSend + "\r\n";
	if (getClientByNick(target))//Privmsg to a client
		return (saveOrSend(*getClientByNick(target), line));
	
	else if (getChannelByNick(target))//PRIVMSG to a channel
	{
		Channel *chan = getChannelByNick(target);
		if(!chan->isMember(client))
			return (saveOrSend(client, ERR_CANNOTSENDTOCHAN(client, target)));
		return (getChannelByNick(target)->broadcastPrivmsg(*this, client, line));
	}
}
