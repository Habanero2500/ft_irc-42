/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 16:14:58 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/18 20:48:31 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/utils/Replies.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"

// PART <channel>{,<channel>} [:<reason>]
void	Server::handlePart(Client &client, const Message &msg)
{
	if (!client.isRegistered())
		return (saveOrSend(client, ERR_NOTREGISTERED(client)));

    const Parser &cmd = msg.getParser();
	const std::vector<std::string>	&params = cmd.params;
	if (params.empty())
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(client, cmd.command)));

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
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(client, name))); // inexisting channel
	Channel	&channel = it->second;
	if (!channel.isMember(client))
		return (saveOrSend(client, ERR_NOTONCHANNEL(client, name))); // not a member on the channel

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

