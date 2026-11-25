/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:56:56 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/24 14:30:22 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <poll.h>
#include <csignal>
#include "Client.hpp"
#include "Channel.hpp"

//Forward declarations
class	Server;
class	Message;
typedef	void	(Server::*CommandHandler)(Client &client, const Message &msg);
struct Parser;

extern bool DEBUG;


struct	KickUtil {//Helpful for the kickClient
	const std::string	channel;
	const std::string	user;
	const std::string	comment;

	KickUtil(const std::string& chan, const std::string& user, const std::string& comment);
};

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

		void	initServerAdress(int port, const std::string& pwd);
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
		std::vector<std::string>	splitComma(const std::string &s) const;
		const std::string			getPrefixForCommand(Client& client) const;
		Channel* 					getChannelByNick(const std::string& name);
		std::vector<std::string>    splitComma(const std::string &s);
		std::string					getArgument(std::vector<std::string>& array, size_t& index);

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
		int		parsingNick(Client &client, const Parser &line);
		void		handleCap(Client &client, const Message &msg);
		void		handleUser(Client &client, const Message& msg);
		void		handlePart(Client &client, const Message& msg);
		void		handleQuit(Client &client, const Message& msg);
		void		handlePrivmsg(Client &client, const Message& msg);
		void		handleDisplay(Client &client, const Message& msg);
		void 		handleInvite(Client& client, const Message& message);
		void		handlePing(Client &client, const Message &msg);
		void		handlePong(Client &client, const Message &msg);
		void		handleWho(Client &client, const Message &msg);

		//Command helpers
		void		quitHelper(Client& client, const std::string& quitLine);

		/******** JOIN AND CHANNELS ********/
		void		handleJoin(Client &client, const Message& msg);
		void		parsingChannelName(Client &client, const std::string& channelName);
		void		createChannel(Client &client, const std::string& channelName);
		void		joinChannel(Client &client, Channel &channel, const std::string& pwd);
		void		joinVerification(Client &client, Channel &channel, const std::string& pwd);
		void		announceJoin(Client &client, Channel &channel);

		/******* MODE *******/
		void		handleMode(Client &client, const Message& msg);
		void		channelMode(Client &client, const Parser &line);
		void		printChannelModes(Client &client, Channel &channel, const std::string &channelName);
		void 		manageChannelModes(Client &client, Channel &channel, const Parser &line);
		void 		addMode(Client &client, Channel &channel, char mode, std::vector<std::string>::const_iterator &it, const std::vector<std::string>::const_iterator &end);
		void 		removeMode(Client &client, Channel &channel, char mode, std::vector<std::string>::const_iterator &it, const std::vector<std::string>::const_iterator &end);
		
		//Operator Commands
		void		kickClient(Client& client, const Message& msg);
		void		singleKick(Client& client, const KickUtil& util);
		void		partChannel(Client &client, const std::string &name, const std::string &reason);
		void		handleTopic(Client& client, const Message& msg);
		bool		topicValidation(Client& client, const Message& msg);

		/**** SIGNALS HANDLERS *******/
		void		initSignals(void);
		static void	signal_handler(int signum);

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
		struct sigaction						sigIntAct;
		struct sigaction						sigPipeAct;
		static bool								interruptFlag;
};
#endif
