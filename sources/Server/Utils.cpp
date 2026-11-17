/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:28:53 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 17:15:13 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"
#include "../../includes/class/Client.hpp"
#include "../../includes/class/Channel.hpp"
#include <sstream>

const std::string	Server::getPrefixForCommand(Client& client) const {
	const std::string prefix = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	return (prefix);
}

Channel*	Server::getChannelByNick(const std::string& name) {
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return (NULL);
	return (&(it->second));
}

std::string	Server::size_tToString(size_t &src) const
{
	std::stringstream	temp;
	temp << src;
	
	std ::string 		result = temp.str(); 
	return result;
}
