/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 20:01:10 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/19 17:14:30 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/class/Channel.hpp"
#include "../../../includes/class/Message.hpp"

void	Server::handleCap(Client &client, const Message &msg) //Capacity negociation 
{
	const std::vector<std::string>	&params = msg.getParser().params;
	if (params.empty())
		return ;
	if (params[0] == "LS") // To know the negociation capacity 
		saveOrSend(client, ":localhost CAP * LS :\r\n"); 
	else if (params[0] == "REQ" && params.size() > 1) // Ask for activation of capabilities to the server
		saveOrSend(client, ":localhost CAP * NAK :" + params[1] + "\r\n"); 
}