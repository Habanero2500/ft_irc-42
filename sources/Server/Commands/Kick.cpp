/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:38:38 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/11 18:52:15 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Channel.hpp"
#include "../../../includes/utils/Replies.hpp"

/*************KICK <channel> <user> ********************/
void	Server::kickClient(Client& client, const Message& msg) {
	if (!client.isRegistered())//451
		return (saveOrSend(client, ERR_NOTREGISTERED(client)));

	const std::vector<std::string>& params = msg.getParser().params;
	if (params.size() < 2)
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(client, std::string("KICK"))));//461
	std::vector<std::string> channels = splitComma(params[0]);
	std::vector<std::string> users = splitComma(params[1]);
	std::string comment = (params.size() >= 3 && !params[2].empty()) ? params[2] : client.getNickname();

	if (channels.size() == 1) {
		for (unsigned int i(0); i < users.size(); ++i) {
			KickUtil util(channels[0], users[i], comment);
			singleKick(client, util);
		}
	}
	else if (channels.size() == users.size()) {
		for (unsigned int i(0); i < users.size(); ++i) {
			KickUtil util(channels[i], users[i], comment);
			singleKick(client, util);
		}
	}
	else
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(client, std::string("KICK"))));
}

//KICK #42,#35 bob,John One-to-One Pairing
void	Server::singleKick(Client& client, const KickUtil& util) {
	std::map<std::string, Channel>::iterator it = _channels.find(util.channel);
	if (it == _channels.end())
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(client, util.channel)));//403

	const Member* member = it->second.getMemberByNick(client.getNickname());
	if (!member)
		return (saveOrSend(client, ERR_NOTONCHANNEL(client, util.channel)));//442
	if (!member->op)
		return (saveOrSend(client, ERR_CHANOPRIVSNEEDED(client, util.channel)));//482

	const Member* target = it->second.getMemberByNick(util.user);
	if (!target)
		return (saveOrSend(client, ERR_USERNOTINCHANNEL(client, util.user, util.channel)));//441

	//Execution + Broadcast
	//:<clientNickname>!<clientUsername>@<clientHostname> KICK <channame> <targetNickname> :\r\n
	const std::string prefix = getPrefixForCommand(client);
	const std::string line = ":" + prefix + " KICK " + util.channel + " " + util.user + " :" + util.comment + "\r\n";

	it->second.broadcast(*this, line);
	it->second.removeClientFromChannel(*target->client);
	if (!it->second.getNumberMember())
		_channels.erase(it);
}
