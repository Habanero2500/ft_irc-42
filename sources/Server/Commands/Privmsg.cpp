/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:36:32 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/11 18:36:44 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"

//PRIVMSG <receiver> :<text to be sent>
void	Server::handlePrivmsg(Client &client, const Message& msg) {
	if (!client.isRegistered())
		return (saveOrSend(client, ERR_NOTREGISTERED()));

	const	std::vector<std::string>& params = msg.getParser().params;
	if (params.empty() || params[0].empty())//411
		return (saveOrSend(client, ERR_NORECIPIENT(msg.getParser().command)));
	if (params.size() < 2 || params[1].empty())//412
		return (saveOrSend(client, ERR_NOTEXTTOSEND()));
	
	const std::string& target = params[0];
	const std::string& textToSend = params[1];
	if (!getClientByNick(target))//401
		return (saveOrSend(client, ERR_NOSUCHNICK(target)));

	//:<sender_nick>!<sender_user>@<sender_host>: PRIVMSG :<msg>\r\n 
	const std::string prefix = getPrefixForCommand(client);
	const std::string line = ":" + prefix + " PRIVMSG " + target + " :" + textToSend + "\r\n";
	saveOrSend(*getClientByNick(target), line);
}