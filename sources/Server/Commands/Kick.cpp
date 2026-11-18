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
	if (!kickValidation(client, msg))
		return ;

	//Execution + Broadcast
	//:<clientNickname>!<clientUsername>@<clientHostname> KICK <channame> <targetNickname> :\r\n
	const std::vector<std::string>& params = msg.getParser().params;
	std::string reason = (params.size() >= 3 && !params[2].empty()) ? params[2] : client.getNickname();
	const std::string prefix = getPrefixForCommand(client);
	const std::string line = ":" + prefix + " KICK " + params[0] + " " + params[1] + " :" + reason + "\r\n";

	Channel* channel = getChannelByNick(params[0]);
	Client* target = getClientByNick(params[1]);
	channel->broadcast(*this, line);
	channel->removeClientFromChannel(*target);
	if (!channel->getNumberMember())
		_channels.erase(params[0]);
}

//KICK #42,#35 bob,John One-to-One Pairing
bool	Server::kickValidation(Client& client, const Message& msg) {
	if (!client.isRegistered())//451
		return (saveOrSend(client, ERR_NOTREGISTERED(client)), false);
	return (true);
}							     

//Il nous faut une fonction pour singleKick, une pour valider le single kick et le maestro apres(kickClient)

void	Server::singleKick(Client& client, const Message& msg) {
	const std::vector<std::string>& params = msg.getParser().params;

	if (params.size() < 2)
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(client, std::string("KICK"))));//461

	const std::string& channel = params[0];
	std::map<std::string, Channel>::iterator it = _channels.find(channel);
	if (it == _channels.end())
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(client, channel)));//403

	const Member* member = it->second.getMemberByNick(client.getNickname());
	if (!member)
		return (saveOrSend(client, ERR_NOTONCHANNEL(client, channel)));//442
	if (!member->op)
		return (saveOrSend(client, ERR_CHANOPRIVSNEEDED(client, channel)));//482

	const std::string& user = params[1];
	if (!it->second.getMemberByNick(user))
		return (saveOrSend(client, ERR_USERNOTINCHANNEL(client, user, channel)));//441
}
