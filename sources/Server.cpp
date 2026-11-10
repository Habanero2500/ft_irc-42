/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/10 18:23:35 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Server.hpp"
#include "../includes/utils/Debug.hpp"
#include "../includes/utils/Replies.hpp"
#include "../includes/class/Client.hpp"
#include "../includes/class/Message.hpp"
#include "../includes/utils/Colors.hpp"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cerrno>


// **** ORTHODOX CANONICAL FORM ****
Server::Server()
{
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("SERVER: Socket failure");
	initCommands();
}

Server::Server(const Server &copy) : _fd(copy._fd){}//A regler

Server	&Server::operator=(const Server &copy)//A regler
{
    if(this != &copy)
    {
        _fd = copy._fd;
    }
    return *this;
}

Server::~Server()
{
	std::cout << "destructor called" << std::endl;
	close(_fd);
}

//********** SERVER INITIATION AND POLL LOOP ************

void	Server::initServerAdress(int port, std::string pwd)
{
	int	opt = 1;

	std::cout << WELCOME_MESSAGE << std::endl;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);

	_pwd = pwd;

	if (bind(_fd, (const sockaddr *)&_address, sizeof(_address)) == -1)
		throw std::runtime_error("SERVER: Bind failure");
	if (listen(_fd, 128) == -1)
		throw std::runtime_error("SERVER: Listen failure");
	
	struct pollfd	serverPollFd;
	serverPollFd.fd = _fd;
	serverPollFd.events = POLLIN;
	serverPollFd.revents = 0;
	
	_listFd.push_back(serverPollFd);
}

void	Server::pollLoop()
{
	while (true)
	{
		if (poll(&_listFd[0], _listFd.size(), -1) == -1)
		{
			if (errno == EINTR)
				continue ;
			else
				throw std::runtime_error("SERVER: Poll failure");
		}
			checkEvents();
	}
}

void	Server::checkEvents()
{
	for(size_t	i = 0; i < _listFd.size(); i++)
	{
		if (_listFd[i].revents & (POLLERR | POLLHUP | POLLNVAL))
		{
			deleteClient(_listFd[i].fd);
			return ;
		}
		if (_listFd[i].revents & POLLIN)
		{
			if (_listFd[i].fd == _fd)
				setNewClient();
			else
				receiveCommunication(_listFd[i].fd);
		}
		if (i< _listFd.size() && (_listFd[i].revents & POLLOUT))
			sendPendingData(_listFd[i].fd);
	}
}

//Setters and Getters
void	Server::setNewClient(void) {
	Client	newClient;
	int		fd;
	
	fd = accept(_fd, 0, 0);
	if (fd == -1)
	{
		std::cerr << "Couldn't accept client";
		return ;
	}
	newClient.setFd(fd);
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Client bind failure";
		close(fd);
		return ;
	}
	std::cout << BLUE "A new client just connected !" RESET << std::endl;
	newClient.setPollFd();
	_listFd.push_back(newClient.getPollFd());
	_clients[newClient.getFd()] = newClient;
}

Client*	Server::getClientByNick(const std::string& nick) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (nick == it->second.getNickname())
			return (&(it->second));
	}
	return (NULL);
}

void	Server::receiveCommunication(int fd)
{
	int		bits;
	char	buffer[1024] = { 0 };
    
	bits = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bits > 0)
	{
		_clients[fd].addBuffer(buffer);
		if (DEBUG)
		{
			std::cout << "Message from client: " << fd  << " " << _clients[fd].getBuffer();
			if (_clients[fd].getBuffer() == "EXIT\n")
			{
				deleteClient(fd);
				close(_fd);
				exit (1);//A voir pour le leaks etc
			}
		}
		parsingBuffer(fd);
		if (_clients.find(fd) != _clients.end() && _clients[fd].shouldDisconnect())
			deleteClient(fd);
	}
}

void	Server::deleteClient(int fd)
{
	close(fd);
	_clients.erase(fd);
	for (std::vector<struct pollfd>::iterator it = _listFd.begin(); it != _listFd.end(); it++)
	{
		if (it->fd == fd)
		{
			_listFd.erase(it);
			return ;
		}
	}
}


/******** COMMANDS PARSING AND GESTION  *******/
void	Server::parsingBuffer(int fd)
{
	Client		&client = _clients[fd];
	std::string	buf = client.getBuffer();
	size_t		pos;

	while ((pos = buf.find('\n')) != std::string::npos)
	{
		std::string line = buf.substr(0, pos);
		buf.erase(0, pos + 1);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (!line.empty())
			executeCommand(client, line);
		if (client.shouldDisconnect())
			break ;
	}
	client.setBuffer(buf);
}


// Command Map Functions
void	Server::initCommands(void) {
	_commandMap["PASS"] = &Server::handlePass;
	_commandMap["NICK"] = &Server::handleNick;
	_commandMap["USER"] = &Server::handleUser;
	// _commandMap["PING"] = &Server::handlePing;
	// _commandMap["QUIT"] = &Server::handleQuit;
	_commandMap["PRIVMSG"] = &Server::handlePrivmsg;
	_commandMap["JOIN"] = &Server::handleJoin;
	_commandMap["INVITE"] = &Server::handleInvite;
	if(DEBUG)
		_commandMap["DISPLAY"] = &Server::handleDisplay;
}

// INVITE <nickname> <channel>
void	Server::handleInvite(Client &client, const Message &msg)
{
	if (!client.isRegistered()) //Deja sur le channel 
		return (saveOrSend(client, ERR_NOTREGISTERED()));
	const std::vector<std::string>	&params = msg.getParser().params;
	if (params.size() < 2)
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(msg.getParser().command)));

	const std::string	targetNick = params[0];
	const std::string	channelName = params[1];
	const std::string	me = client.getNickname();

	std::map<std::string, Channel>::iterator chanIt = _channels.find(channelName);
	if (chanIt == _channels.end())
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(channelName)));
	Channel	&channel = chanIt->second;

	if (!channel.isMember(client.getFd())) //Si celui qi fait l'invit n'est pas membre
		return (saveOrSend(client, ERR_NOTONCHANNEL(channelName)));
	if (!channel.isOperator(client.getFd())) //s'il n'est pas chouffin (operator)
		return (saveOrSend(client, ERR_CHANOPRIVSNEEDED(channelName)));

	Client	*target = getClientByNick(targetNick);
	if (!target) //Invite n'existe pas
		return (saveOrSend(client, ERR_NOSUCHNICK(targetNick))); 
	if (channel.isMember(target->getFd())) //si l'invite est deja sur le channel
		return (saveOrSend(client, ERR_USERONCHANNEL(targetNick, channelName))); 

	channel.addInvite(targetNick); //on l'invite
}

void Server::handleDisplay(Client& client, const Message& message)
{
	(void)message;
	client.displayClient();
}


void	Server::executeCommand(Client& client, const std::string& rawline) {
	Message msg;
	std::string	input = rawline;
	msg.parsing(input);

	std::string	command = msg.getParser().command;
	if (command.empty())
		return ;

	std::map<std::string, CommandHandler>::iterator it = _commandMap.find(command);
	if (it != _commandMap.end())
		(this->*(it->second))(client, msg);
	else
		return(saveOrSend(client, ERR_UNKNOWNCOMMAND(command)));//No match found
}

void	Server::handlePass(Client &client, const Message& msg)
{
	Parser pars = msg.getParser();  
	if(client.getAttempt() > 2) //Trop de tentatives
	{
		if(DEBUG)
			std::cout << "Too much attempts" << std::endl;
		deleteClient(client.getFd());
		return;
	}
	if (pars.params.empty())
    {
		std::string cmd = "PASS";
        std::string message = ERR_NEEDMOREPARAMS(cmd);
        send(client.getFd(), message.c_str(), message.size(), 0);
        return;
    }
	if(client.getPass() == true) //pwd deja rentre
  	{
		if(DEBUG)
			std::cout << "Already registered" << std::endl;
        std::string message = ERR_ALREADYREGISTRED();
        send(client.getFd(), message.c_str(), message.size(), 0);
    	return;
    }
	if(pars.params[0] != _pwd)
	{
		if(DEBUG)
			std::cout << "Wrong password" << std::endl;
		std::string message = ERR_PASSWDMISMATCH();
        send(client.getFd(), message.c_str(), message.size(), 0);
		client.increaseAttempt();
		return;	
	}
	if(pars.params.size() < 1) // pas d'arg
	{
		if(DEBUG)
			std::cout << "Argument missing" << std::endl; 
		std::string msg = "PASS";
		std::string message = ERR_NEEDMOREPARAMS(msg);
        send(client.getFd(), message.c_str(), message.size(), 0);
		return;
	}
	else if(pars.params[0] == _pwd)
	{
		if(DEBUG)
			std::cout << "PWD OK" << std::endl; 
		client.setPass();
		return;	
	}
}
void	Server::handleNick(Client &client, const Message& msg)
{
	Parser	line = msg.getParser();
	
	std::string	Nickname = parsingNick(client, line);
	if ((Nickname.empty()))
		return;

	client.setNickname(Nickname);
	client.setNick();
	if (DEBUG)
	{
		std::cout << formatDebugBuffer(client.getNickname()) << std::endl;
	std::string message = "Nick ok\r\n";
    send(client.getFd(), message.c_str(), message.size(), 0);
	}
	isRegistered(client);
		
}

std::string	Server::parsingNick(Client &client, const Parser &line)
{
	if (line.params.size() == 0)
	{
		std::string message = ERR_NONICKNAMEGIVEN();
        send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	std::string	NicknameGiven = *(line.params.begin());

	if (NicknameGiven.find_first_not_of(std::string(LETTERS) + NUMBERS + SPECIAL_IRC) != std::string::npos 
		|| NicknameGiven.size() > 9 || isdigit(NicknameGiven[0]) || NicknameGiven[0] == '-')
	{
		std::string message = ERR_ERRONEUSNICKNAME(NicknameGiven);
        send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first == client.getFd()) // On compare pas avec le premier 
            continue;
		if (it->second.getNickname() == NicknameGiven)
		{
			std::string message = ERR_NICKNAMEINUSE(NicknameGiven);
        	send(client.getFd(), message.c_str(), message.size(), 0);
			return "";
		}
	}
	return (NicknameGiven);
}

#define USERLEN 16      // longueur max conservée, valeur au choix

static bool isValidUsername(const std::string &s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c <= ' ' || c == 127 || c == '@' || c == '!')
			return false;
	}
	return true;
}

static bool isValidRealname(const std::string &s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c < ' ' || c == 127)
			return false;
	}
	return true;
}

void	Server::handleUser(Client &client, const Message& msg)
{
	if(client.getPass() != true) 
	{
		if(DEBUG)
			std::cout << "Password has to be set before anything" << std::endl;
		return;
	}
	Parser pars = msg.getParser();
	if (client.getUser() == true) {
		if(DEBUG)
			std::cout << "Already registered" << std::endl;
		std::string msg = ERR_ALREADYREGISTRED();
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		return;

	}
	else if (pars.params.size() < 4) {
		if(DEBUG)
			std::cout << "Need more params" << std::endl;
		std::string cmd = "USER";
		std::string msg = ERR_NEEDMOREPARAMS(cmd);
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		return;
	}
	std::string			username = pars.params[0];    // USER <username> <mode> <unused> :<realname>
	const std::string	&realname = pars.params[3];              
	if (!isValidUsername(username) || !isValidRealname(realname))
	{
		if (DEBUG)
			std::cout << "Wrong argument --> " << username << " / " << realname << std::endl;
		return ;
	}
	client.setUsername(username);
	client.setRealName(realname);
	client.setHostname(pars.params[1]);
	client.setServername(pars.params[2]);
	client.setUser();
	if(DEBUG)
			std::cout << "USER ok" << std::endl;
	isRegistered(client);
}
void Server::isRegistered(Client &client)
{
	if(client.getNick() == true && client.getUser() == true && client.getPass() == true)
	{
		client.setRegistered();
		std::cout << BLUE "A new client just registered !" RESET << std::endl;
	}
}

//Message Transmission
void	Server::saveOrSend(Client& client, const std::string& data) {
	if (!client.isSendBufferEmpty()) {//when _sendBuffer not empty
		client.appendToSendBuffer(data);
		return (enablePollout(client.getFd()));
	}
	ssize_t bytesSent = send(client.getFd(), data.c_str(), data.size(), 0);
	if (bytesSent >= 0 && static_cast<size_t>(bytesSent) == data.size())
		return ;//Everything was sent
	if (bytesSent > 0)
		client.appendToSendBuffer(data.substr(bytesSent));//Partial sent
	else
		client.appendToSendBuffer(data);//Nothing was sent
	enablePollout(client.getFd());
}

void	Server::enablePollout(int clientFd) {
	for (size_t i = 0; i < _listFd.size(); ++i) {
		if (clientFd == _listFd[i].fd) {
			_listFd[i].events |= POLLOUT;
			return ;
		}
	}
}

void	Server::disablePollout(int clientFd) {
	for (size_t i = 0; i < _listFd.size(); ++i) {
		if (clientFd == _listFd[i].fd) {
			_listFd[i].events &= ~POLLOUT;
			return ;
		}
	}
}

void	Server::sendPendingData( int fd) {
	Client &client = _clients[fd];
	std::string &buffer = client.getSendBuffer();

	if (!buffer.empty()) {
		ssize_t bytes = send(fd, buffer.c_str(), buffer.size(), 0);
		if (bytes > 0)
			buffer.erase(0, bytes);
		else if (bytes == -1) {
			deleteClient(fd);
			return ;
		}
	}
	if (buffer.empty())
		disablePollout(fd);
}

//PRIVMSG <receiver> :<text to be sent>
void	Server::handlePrivmsg(Client &client, const Message& msg) {
	if (!client.isRegistered()) //a modifier
		return (saveOrSend(client, ERR_NOTREGISTERED()));

	const	std::vector<std::string>& params = msg.getParser().params;
	if (params.empty() ||params[0].empty())//411
		return(saveOrSend(client, ERR_NORECIPIENT(msg.getParser().command)));
	if (params.size() < 2 || params[1].empty())//412
		return (saveOrSend(client, ERR_NOTEXTTOSEND()));
	
	const std::string& target = params[0];
	const std::string& textToSend = params[1];
	if (!getClientByNick(target))//401
		return (saveOrSend(client, ERR_NOSUCHNICK(target)));
	//:<sender_nick>!<sender_user>@<sender_host>: PRIVMSG :<msg>\r\n 
	
	std::string prefix = client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	std::string line = ":" + prefix + " PRIVMSG " + target + " :" + textToSend + "\r\n";
	saveOrSend(*getClientByNick(target), line);
}

/***************** JOIN & CHANNELS *********************/
void	Server::handleJoin(Client &client, const Message& msg)
{
	Parser	line = msg.getParser();
	std::string	ChannelName = parsingChannelName(client, line);
	if ((ChannelName.empty()))
		return;
	
	if (client.getNumberChannels() > 10)
	{
		std::string message = ERR_TOOMANYCHANNELS(ChannelName);
        send(client.getFd(), message.c_str(), message.size(), 0);
	}
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->second.getName() == ChannelName)
			return (joinChannel(client, it->second));
	}
	createChannel(client, ChannelName);
}

std::string	Server::parsingChannelName(Client &client, const Parser &line)
{
	if (line.params.size() == 0)
	{
		std::string command = "JOIN";
		std::string message = ERR_NEEDMOREPARAMS(command);
        send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	std::string	ChannelName = *(line.params.begin());

	if (ChannelName[0] != '@' || ChannelName[0] != '#' || ChannelName.size() > 200
		|| ChannelName.size() < 2 || ChannelName.find_first_of("\a, ") != std::string::npos)
	{
		std::string message = ERR_NOSUCHCHANNEL(ChannelName);
        send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	return (ChannelName);
}

void	Server::createChannel(Client &client, std::string channelName)
{
	Channel	newChannel(channelName);

	newChannel.addMember(client);
	newChannel.setOperator(client);
	_channels[channelName] = newChannel;
	std::string message = NEWMEMBERMESSAGE(client.getNickname());
    send(client.getFd(), message.c_str(), message.size(), 0);
	if (DEBUG)
	{
		std::cout << "current channels :" << std::endl;
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			std::cout <<it->second.getName() << std::endl;
	}
}

void	Server::joinChannel(Client &client, Channel &channel)
{
	try
	{
		joinVerification(client, channel);
		channel.addMember(client);
		std::string message = NEWMEMBERMESSAGE(client.getNickname());
    	send(client.getFd(), message.c_str(), message.size(), 0);
		if (DEBUG)
		{
			std::cout << BLUE "channel: " << channel.getName() << " accepted new member: " << client.getNickname() << RESET << std::endl;
			// std::cout << "current members :" << std::endl;
			// for (size_t i = 0; i < channel.getNumberMember(); i++)
			// {
			// 	std::cout << channel.getMember().client.getNickname() << std::endl;
			// }
		}
	}
	catch (int e)
	{
		return ;
	}
}

void	Server::joinVerification(Client &client, Channel &channel)
{
	if (channel.isModeActive(I))
	{
		std::string message = ERR_INVITEONLYCHAN(channel.getName());
        send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
	if (channel.isModeActive(K))
	{
		std::string message = ERR_BADCHANNELKEY(channel.getName());
        send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
	if (channel.isModeActive(L) && channel.isMembersLimitReached())
	{
		std::string message = ERR_CHANNELISFULL(channel.getName());
        send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
	if (channel.isMemberBanned(client))
	{
		std::string message = ERR_BANNEDFROMCHAN(channel.getName());
        send(client.getFd(), message.c_str(), message.size(), 0);
		throw -1;
	}
}
