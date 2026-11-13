/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 17:46:19 by opapouts          #+#    #+#             */
/*   Updated: 2026/11/13 17:46:20 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/class/Server.hpp"
#include "../../../includes/class/Client.hpp"
#include "../../../includes/class/Message.hpp"
#include <vector>
#include <set>


//1) Close the connection
//2) Acknowledge with ERROR 
//3) Relay QUIT to Peers(if registered)
//QUIT [<Quit message>]
void	Server::handleQuit(Client& client, const Message& msg) {
	const std::vector<std::string> &params = msg.getParser().params;

	std::string	quitMsg;
	if (!params.empty() && !params[0].empty())
		quitMsg = params[0];
	else {
		if (!client.getNickname().empty())
			quitMsg = client.getNickname();
		else//Case where he is registered but no nickname
			quitMsg = "Client Quit";
	}
	if (client.isRegistered()) {
		std::string quitLine = ":" + getPrefixForCommand(client) + " QUIT :" + quitMsg + "\r\n";
		quitHelper(client, quitLine);
		}
	std::string errorReply = "ERROR :Closing Link: " + client.getHostname() + " (" + quitMsg + ")\r\n";
	send(client.getFd(), errorReply.c_str(), errorReply.size(), 0);

	client.setToDisconnect();
}
	
//This function will broadcast the quitLine to every member that this
//client is part of and then will quit the channel
void	Server::quitHelper(Client& client, const std::string& quitLine) {
	std::set<int> checkedFds;
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end();) {
		if (it->second.getMemberByNick(client.getNickname())) {
			it->second.broadcastQuit(*this, client, quitLine, checkedFds);
			it->second.removeClientFromChannel(client);
			if (!it->second.getNumberMember()) {
				std::map<std::string, Channel>::iterator deleteIt = it++;
				_channels.erase(deleteIt);
				continue ;
			}
		}
		++it;
	}
}
