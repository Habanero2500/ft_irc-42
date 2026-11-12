/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 20:01:10 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/12 20:10:09 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/class/Channel.hpp"

void Server::handleCap(Client &client, const Message &msg)
{
    (void)msg;
    saveOrSend(client, ":localhost CAP * LS :\r\n");
}