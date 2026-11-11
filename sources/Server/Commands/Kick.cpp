/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:38:38 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/11 18:38:57 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"

/*************KICK <channel> <user> ********************/
void	Server::kickClient(Client& client, const Message& msg) {
	if (!kickValidation(client, msg))
		return ;

	//Execution + Broadcast
	//:<clientNickname>!<clientUsername>@<clientHostname> KICK <channame> <targetNickname> :\r\n
	const std::vector<std::string>& params = msg.getParser().params;
	const std::string prefix = getPrefixForCommand(client);
	const std::string line = ":" + prefix + " KICK " + params[0] + " " + params[1] + "\r\n";

	Channel* channel = getChannelByNick(params[0]);
	Client* target = getClientByNick(params[1]);
	channel->broadcast(*this, line);
	channel->removeClientFromChannel(*target);
	if (!channel->getNumberMember())
		_channels.erase(params[0]);
}

//peut etre ajouter ca dans la map avec les commandes
bool	Server::kickValidation(Client& client, const Message& msg) {
	if (!client.isRegistered())//451
		return (saveOrSend(client, ERR_NOTREGISTERED()), false);

	const std::vector<std::string>& params = msg.getParser().params;
	if (params.size() < 2)
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(std::string("KICK"))), false);//461
								     
	const std::string& channel = params[0];
	const std::string& user = params[1];

	std::map<std::string, Channel>::iterator it = _channels.find(channel);
	if (it == _channels.end())//maybe I can add getChannelByNick ici
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(channel)), false);//403

	const Member* member = it->second.getMemberByNick(client.getNickname());
	if (!member)
		return (saveOrSend(client, ERR_NOTONCHANNEL(channel)), false);//442
	if (!member->op)
		return (saveOrSend(client, ERR_CHANOPRIVSNEEDED(channel)), false);//482
	if (!it->second.getMemberByNick(user))
		return (saveOrSend(client, ERR_USERNOTINCHANNEL(user, channel)), false);//441
	return (true);
}