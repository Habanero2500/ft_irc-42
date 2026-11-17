/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 16:14:58 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/17 19:02:57 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/utils/Replies.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"



static std::vector<std::string>	splitComma(const std::string &s)
{
    if (!client.isRegistered())
        return (saveOrSend(client, ERR_NOTREGISTERED(client)));
    
    Parser pars = msg.getParser();
    if (pars.params.empty()) // No parameter
        return (saveOrSend(client, ERR_NEEDMOREPARAMS(client, pars.command)));
        
    std::map<std::string, Channel>::iterator chanIt =
        _channels.find(pars.params[0]);

    if (chanIt == _channels.end()) // Impossible to find
        return (saveOrSend(client, ERR_NOSUCHCHANNEL(client, pars.params[0])));
    Channel &channel = chanIt->second;
    if (!channel.isMember(client)) // Client is not a member of the channel
        return (saveOrSend(client, ERR_NOTONCHANNEL(client, channel.getName())));

    // if (params.size() >= 2)
    //     reason = params[1];

    std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " PART " + chanIt->first + " :Leaving" + "\r\n";
    channel.broadcast(*this, message); //send the message to the channel member
    channel.removeClientFromChannel(client);
    if (channel.getNumberMember() == 0) //delete the channel if there is no member anymore
        _channels.erase(chanIt);
    client.decreaseNumberChannels();
	std::vector<std::string>	res;
	size_t						start = 0, pos;

	while ((pos = s.find(',', start)) != std::string::npos)
	{
		res.push_back(s.substr(start, pos - start));
		start = pos + 1;
	}
	res.push_back(s.substr(start));
	return res;
}

// PART <channel>{,<channel>} [:<reason>]
void	Server::handlePart(Client &client, const Message &msg)
{
	if (!client.isRegistered())
		return (saveOrSend(client, ERR_NOTREGISTERED()));

    const Parser &cmd = msg.getParser();
	const std::vector<std::string>	&params = cmd.params;
	if (params.empty())
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(cmd.command)));

	std::vector<std::string>	names = splitComma(params[0]);
	std::string					reason = (params.size() > 1) ? params[1] : "";

	for (size_t i = 0; i < names.size(); i++)
		if (!names[i].empty())
			partChannel(client, names[i], reason);
}

void	Server::partChannel(Client &client, const std::string &name, const std::string &reason)
{
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(name))); // inexisting channel
	Channel	&channel = it->second;
	if (!channel.isMember(client))
		return (saveOrSend(client, ERR_NOTONCHANNEL(name))); // not a member on the channel

	std::string	line = ":" + client.getNickname() + "!" + client.getUsername() + "@"
		+ client.getHostname() + " PART " + name;
	if (!reason.empty())
		line += " :" + reason;
	line += "\r\n";

	channel.broadcast(*this, line);
	channel.removeClientFromChannel(client); //A tester
	if (channel.isEmpty())
		_channels.erase(it);
}

