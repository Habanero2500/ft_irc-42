/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:28:53 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 18:58:03 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"
#include "../../includes/class/Channel.hpp"


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

KickUtil::KickUtil(const std::string& chan, const std::string& user, const std::string& comment) : channel(chan), user(user), comment(comment) {
	return ;
}
