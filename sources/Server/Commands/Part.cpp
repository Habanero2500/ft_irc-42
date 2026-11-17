/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 16:14:58 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/16 22:48:03 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/utils/Replies.hpp"


void	Server::handlePart(Client &client, const Message& msg)
{
    if (!client.isRegistered())
        return (saveOrSend(client, ERR_NOTREGISTERED()));
    
    Parser pars = msg.getParser();
    if (pars.params.empty()) // No parameter
        return (saveOrSend(client, ERR_NEEDMOREPARAMS(pars.command)));
        
    std::map<std::string, Channel>::iterator chanIt = _channels.find(pars.params[0]);

    if (chanIt == _channels.end()) // Impossible to find
        return (saveOrSend(client, ERR_NOSUCHCHANNEL(pars.params[0])));
    Channel &channel = chanIt->second;
    if (!channel.isMember(client)) // Client is not a member of the channel
        return (saveOrSend(client, ERR_NOTONCHANNEL(channel.getName())));

    // if (params.size() >= 2)
        // reason = params[1];

    std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " PART " + chanIt->first + " : Leaving" + "\r\n";
    channel.broadcast(*this, message); //send the message to the channel member
    channel.removeClientFromChannel(client);
    if (channel.getNumberMember() == 0) //delete the channel if there is no member anymore
        _channels.erase(chanIt);
    client.decreaseNumberChannels();
}
