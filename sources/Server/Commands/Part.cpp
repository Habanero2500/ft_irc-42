/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 16:14:58 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/17 15:50:12 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/utils/Replies.hpp"
#include "../../../includes/class/Message.hpp"
#include "../../../includes/class/Client.hpp"



static std::vector<std::string>	splitComma(const std::string &s)
{
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

