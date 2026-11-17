/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:28:53 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/17 18:20:51 by malaimo          ###   ########.fr       */
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

std::string	Server::size_tToString(size_t src) const
{
	std::stringstream	temp;
	temp << src;
	
	std ::string 		result = temp.str(); 
	return result;
}

std::vector<std::string>    Server::splitComma(const std::string &s)
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

std::string    Server::getArgument(std::vector<std::string> array, size_t index)
{
	if (index < array.size())
		return (array[index]);
	return ("");
}
