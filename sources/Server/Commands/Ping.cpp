/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 14:22:22 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/24 12:10:26 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Message.hpp"

void	Server::handlePing(Client &client, const Message &msg)
{
	const std::vector<std::string>	&params = msg.getParser().params;

	if (params.empty())
		return (saveOrSend(client, ":localhost 409 " + client.getNickname() + " :No origin specified\r\n"));
	saveOrSend(client, ":localhost PONG localhost :" + params[0] + "\r\n");//peut etre ajouter un retour dans replies.hpp
}

void	Server::handlePong(Client &client, const Message &msg)
{
	(void)client;
	(void)msg;
}
void	Server::handleWho(Client &client, const Message &msg)
{
	(void)client;
	(void)msg;	
	return;
}
