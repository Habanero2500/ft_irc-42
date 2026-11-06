/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/06 17:20:14 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>
#include "Channel.hpp"

//Forward declarations
class	Message;
class	Client;
class	Server;
typedef	void	(Server::*CommandHandler)(Client &client, const Message &msg);
struct Parser;

extern bool DEBUG;

#define NUMBERS "0123456789"
#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SPECIAL_IRC "[]\\_^{|}"

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
		//Command Map Functions
		void	initCommands(void);
		void	executeCommand(Client& client, const std::string& rawline);
		//Message transmission
		void	saveOrSend(Client& client, const std::string& data);
		void	enablePollout(int clientFd);
		void	disablePollout(int clientFd);
		void	sendPendingData(int fd);


		//Command Handlers
		void		handlePass(Client &client, const Message& msg);
		
		void		handleNick(Client &client, const Message& msg);
		std::string	parsingNick(Client &client, const Parser &line);
		
		void		handleUser(Client &client, const Message& msg);
		// void	handlePing(Client &client, const Message& msg);
		// void	handleQuit(Client &client, const Message& msg);
		void		handlePrivmsg(Client &client, const Message& msg);
		
		/******** JOIN AND CHANNELS ********/
		void		handleJoin(Client &client, const Message& msg);
		std::string	parsingChannelName(Client &client, const Parser &line);
		void		createChannel(Client &client, std::string channelName);
		void		joinChannel(Client &client, Channel &channel);
		void		joinVerification(Client &client, Channel &channel);
		
	private:
		int						_fd;
		struct sockaddr_in 				_address;
		std::map<int, Client>				_clients;
		std::map<std::string, Channel>			_channels;
		std::map<std::string, CommandHandler> 		_commandMap;
		std::vector<struct pollfd>			_listFd;
		std::string					_pwd;
		int						_port;
};
#endif
