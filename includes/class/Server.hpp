/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 22:13:31 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"

#include "../utils/Colors.hpp"
#include "../utils/Replies.hpp"

#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cerrno>
#include <sstream>

//Forward declarations
class	Server;
class	Message;
class	Client;
typedef	void	(Server::*CommandHandler)(Client &client, const Message &msg);
struct Parser;

extern bool DEBUG;

#define NUMBERS "0123456789"
#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SPECIAL_IRC "[]\\_^{|}-"

class	Server
{	
	public:
		//Orthodox Canonical Form
		Server();
		Server(const Server &copy);
		Server	&operator=(const Server &copy);
		~Server();

		void	initServerAdress(int port, std::string pwd);//a voir
		void	pollLoop();
		void	checkEvents(void);
		void	receiveCommunication(int fd);
		void	deleteClient(int fd);
		void	parsingBuffer(int fd);
		void	executeCommand(int fd, const std::string &line);

		//Setters and Getters
		void	setNewClient(void);
		Client*	getClientByNick(const std::string& nick);

		//***** Helper Functions for Commands *****//
		const std::string	getPrefixForCommand(Client& client) const;
		Channel* 			getChannelByNick(const std::string& name);
		std::string			size_tToString(size_t &src) const;

		//Command Map Functions
		void	initCommands(void);
		void	executeCommand(Client& client, const std::string& rawline);
		//Message transmission
		void	saveOrSend(Client& client, const std::string& data);
		void	enablePollout(int clientFd);
		void	disablePollout(int clientFd);
		void	sendPendingData(int fd);
		void	isRegistered(Client &client);


		//Command Handlers
		void		handlePass(Client &client, const Message& msg);
		void		handleNick(Client &client, const Message& msg);
		int			parsingNick(Client &client, const Parser &line);

		void		handleCap(Client &client, const Message &msg);
		void		handleUser(Client &client, const Message& msg);
		// void	handlePing(Client &client, const Message& msg);
		// void	handleQuit(Client &client, const Message& msg);
		void		handlePrivmsg(Client &client, const Message& msg);
		void		handleDisplay(Client &client, const Message& msg);
		void 		handleInvite(Client& client, const Message& message);
		
		/******** JOIN AND CHANNELS ********/
		void		handleJoin(Client &client, const Message& msg);
		std::string	parsingChannelName(Client &client, const Parser &line);
		void		createChannel(Client &client, std::string channelName);
		void		joinChannel(Client &client, Channel &channel);
		void		joinVerification(Client &client, Channel &channel);
		/******* MODE *******/
		void		handleMode(Client &client, const Message& msg);
		void		channelMode(Client &client, const Parser &line);
		void		printChannelModes(Client &client, Channel &channel, const std::string &channelName);
		void 		addChannelModes(Client &client, Channel &channel, const Parser &line);
		void		userMode(Client &client, const Parser &line);
		
		//Operator Commands	
		void		kickClient(Client& client, const Message& msg);
		bool		kickValidation(Client& client, const Message& msg);

		/******* UTILS *******/

		
	private:
		int										_fd;
		struct sockaddr_in 						_address;
		std::map<int, Client>					_clients;
		std::map<std::string, Channel>			_channels;
		std::map<std::string, CommandHandler> 	_commandMap;
		std::vector<struct pollfd>				_listFd;
		std::string								_pwd;
		int										_port;
};
#endif
