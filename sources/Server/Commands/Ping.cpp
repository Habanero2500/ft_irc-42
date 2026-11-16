/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 14:22:22 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/16 14:28:21 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"

void	Server::handlePing(Client &client, const Message &msg)
{
	const std::vector<std::string>	&params = msg.getParser().params;

	if (params.empty())
		return (saveOrSend(client, ":localhost 409 " + client.getNickname() + " :No origin specified\r\n"));
	saveOrSend(client, ":localhost PONG localhost :" + params[0] + "\r\n");
}

void	Server::handlePong(Client &client, const Message &msg)
{
	(void)client;
	(void)msg;
}