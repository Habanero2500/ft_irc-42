/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/06 13:42:36 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Message.hpp"


#include <ctype.h>
class	Server;//Forward declaration for the typedef below
typedef	void	(Server::*CommandHandler)(Client &client, const Message &msg);

extern bool DEBUG;

#define NUMBERS "0123456789"
#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SPECIAL_IRC "[]\\_^{|}"

class	Server
{
	private:
	int							_fd;
	struct sockaddr_in 			_adress;
	std::map<int, Client>		_clients;
	std::vector<struct pollfd>	_listFd;
	std::string					_pwd;
	int							_port;
	std::map<std::string, CommandHandler> 	_commandMap;
	
	public:
		//Orthodox Canonical Form
		Server();
		Server(const Server &copy);
		Server	&operator=(const Server &copy);
		~Server();

		void	initServerAdress(int port, std::string pwd);
		void	pollLoop();
		void	checkEvents();
		void	setNewClient();
		void	receiveCommunication(int fd);
		void	deleteClient(int fd);
		void	parsingBuffer(int fd);
		void	executeCommand(int fd, const std::string &line);
		//Command Map Functions
		void	initCommands(void);
		void	executeCommand(Client& client, const std::string& rawline);

		
		//Command Handlers
		void	handlePass(Client &client, const Message& msg);
		
		void		handleNick(Client &client, const Message& msg);
		std::string	parsingNick(Client &client, const Parser &line);
		
		void	handleUser(Client &client, const Message& msg);
		// void	handlePing(Client &client, const Message& msg);
		// void	handleQuit(Client &client, const Message& msg);
		// void	handlePrivmsg(Client &client, const Message& msg);
};
#endif
