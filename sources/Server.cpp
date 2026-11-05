/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/05 16:57:35 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Server.hpp"
#include "../includes/utils/Debug.hpp"

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
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(port);
	
	_pwd = pwd;
	
	if (bind(_fd, (const sockaddr *)&_adress, sizeof(_adress)) == -1)
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
		for(size_t	i = 0; i < _listFd.size(); i++)
			checkEvents(&i);
	}
}

void	Server::checkEvents(size_t *i)
{
	if (_listFd[*i].revents & (POLLERR | POLLHUP | POLLNVAL))
	{
		deleteClient(_listFd[*i].fd);
		return ;
	}
	if (_listFd[*i].revents & POLLIN)
	{
		if (_listFd[*i].fd == _fd)
			setNewClient();
		else
			receiveCommunication(_listFd[*i].fd);
	}
}

void	Server::setNewClient()
{
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
				exit (1);
			}
		}
		parsingBuffer(fd);
	}	
	else if (bits == 0 || bits == -1)
		deleteClient(fd);
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
	}
	client.setBuffer(buf);
}

//Command Map Functions
void	Server::initCommands(void) {
	_commandMap["PASS"] = &Server::handlePass;
	_commandMap["NICK"] = &Server::handleNick;
	_commandMap["USER"] = &Server::handleUser;
	// _commandMap["PING"] = &Server::handlePing;
	// _commandMap["QUIT"] = &Server::handleQuit;
	// _commandMap["PRIVMSG"] = &Server::handlePrivmsg;
	_commandMap["JOIN"] = &Server::handleJoin;
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
	{
		// std::string message = ERR_UNKNOWNCOMMAND(command);
        // send(client.getFd(), message.c_str(), message.size(), 0);
		return ;
	}
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
	if(client.getPass() == true) //pwd deja rentre
  	{
		if(DEBUG)
			std::cout << "Already registered" << std::endl;
        std::string message = ERR_ALREADYREGISTRED();
        send(client.getFd(), message.c_str(), message.size(), 0);
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
	if(pars.params[0] != _pwd)
	{
		if(DEBUG)
			std::cout << "Wrong password" << std::endl;
		client.increaseAttempt();
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
		std::cout << formatDebugBuffer(client.getNickname()) << std::endl;
	std::string message = "Nick ok\r\n";
	send(client.getFd(), message.c_str(), message.size(), 0);
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
		|| NicknameGiven.size() > 9 || isalpha(NicknameGiven[0]) || NicknameGiven[0] == '-'
		|| NicknameGiven.size() < 1)
	{
		std::string message = ERR_ERRONEUSNICKNAME(NicknameGiven);
        send(client.getFd(), message.c_str(), message.size(), 0);
		return "";
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second.getNickname() == NicknameGiven)
		{
			std::string message = ERR_NICKNAMEINUSE(NicknameGiven);
        	send(client.getFd(), message.c_str(), message.size(), 0);
			return "";
		}
	}
	return (NicknameGiven);
}


void	Server::handleUser(Client &client, const Message& msg)
{
	(void)client;
	(void)msg;
	std::cout << "User ok" << std::endl;
}



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
		{
			joinChannel(client, it->second);
			return ;
		}
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
	_channels[channelName] = newChannel;
}

void	Server::joinChannel(Client &client, Channel &channel)
{
	
}