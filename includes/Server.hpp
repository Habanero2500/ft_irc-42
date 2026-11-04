/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/02 16:49:02 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>

//Forward declarations
class	Message;
class	Client;
class	Server;
typedef	void	(Server::*CommandHandler)(Client &client, const Message &msg);

extern bool DEBUG;

class	Server
{
	
	public:
		//Orthodox Canonical Form
		Server();
		Server(const Server &copy);
		Server	&operator=(const Server &copy);
		~Server();

		void	initServerAdress(int port);
		void	pollLoop();
		void	checkEvents(void);//Modifie
		void	setNewClient();
		void	receiveCommunication(size_t i ,int fd);//Modifie
		void	deleteClient(size_t i, int fd);//Modifie
		void	parsingBuffer(int fd);
		void	executeCommand(int fd, const std::string &line);
		//Command Map Functions
		void	initCommands(void);
		void	executeCommand(Client& client, const std::string& rawline);
		//Message transmission
		void	saveOrSend(Client& client, const std::string& data);
		void	enablePollout(int clientFd);
		void	disablePollout(int clientFd);

	private:
		int					_fd;
		struct sockaddr_in 			_address;
		std::map<int, Client>			_clients;
		std::map<std::string, CommandHandler> 	_commandMap;
		std::vector<struct pollfd>		_listFd;
		//Command Handlers
		void	handlePass(Client &client, const Message& msg);
		void	handleNick(Client &client, const Message& msg);
		void	handleUser(Client &client, const Message& msg);
		void	handlePing(Client &client, const Message& msg);
		void	handleQuit(Client &client, const Message& msg);
		void	handlePrivmsg(Client &client, const Message& msg);
};
#endif
