/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 18:45:05 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/12 20:01:58 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/class/Channel.hpp"

void Server::handleInvite(Client &client, const Message &msg)
{
    if (!client.isRegistered())
        return (saveOrSend(client, ERR_NOTREGISTERED()));

    const std::vector<std::string> &params = msg.getParser().params;
    if (params.size() < 2)
        return (saveOrSend(client, ERR_NEEDMOREPARAMS(msg.getParser().command)));

    const std::string targetNick = params[0];
    const std::string channelName = params[1];
    const std::string me = client.getNickname();

    std::map<std::string, Channel>::iterator chanIt = _channels.find(channelName);
    if (chanIt == _channels.end())
        return (saveOrSend(client, ERR_NOSUCHCHANNEL(channelName)));

    Channel &channel = chanIt->second;

    if (!channel.isMember(client))
        return (saveOrSend(client, ERR_NOTONCHANNEL(channelName)));
    if (!channel.isOperator(client))
        return (saveOrSend(client, ERR_CHANOPRIVSNEEDED(channelName)));

    Client *target = getClientByNick(targetNick);
    if (!target)
        return (saveOrSend(client, ERR_NOSUCHNICK(targetNick)));
    if (channel.isMember(*target))
        return (saveOrSend(client, ERR_USERONCHANNEL(targetNick, channelName)));

    channel.addMember(*target);
    saveOrSend(client, RPL_INVITING(me, targetNick, channelName));
    saveOrSend(*target, ":" + me + "!" + client.getUsername() + "@" + client.getHostname()
        + " INVITE " + targetNick + " " + channelName + "\r\n");
}
