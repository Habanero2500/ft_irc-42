/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 13:53:59 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/04 14:40:12 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/Server.hpp"

// **** ORTHODOX CANONICAL FORM ****

Server::Server()
{
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("SERVER: Socket failure");
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
		deleteClient(i, _listFd[*i].fd);
		return ;
	}
	if (_listFd[*i].revents & POLLIN)
	{
		if (_listFd[*i].fd == _fd)
			setNewClient();
		else
			receiveCommunication(i, _listFd[*i].fd);
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

void	Server::receiveCommunication(size_t *i, int fd)
{
	int		bits;
	char	buffer[1024] = { 0 };
    
	bits = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bits > 0)
	{
		_clients[fd].addBuffer(buffer);
		if (_clients[fd].getBuffer().find_first_not_of("\r\n") != std::string::npos)
			parsingBuffer(fd);
		if (DEBUG)
		{
			std::cout << "Message from client: " << fd  << " " << _clients[fd].getBuffer();
			if (_clients[fd].getBuffer() == "EXIT\n")
			{
				deleteClient(i, fd);
				close(_fd);
				exit (1);
			}
		}
	}
	else if (bits == 0 || bits == -1)
		deleteClient(i, fd);
}

void	Server::deleteClient(size_t *i, int fd)
{
	close(fd);
	_clients.erase(fd);
	_listFd.erase(_listFd.begin() + *i);
	(*i)--;
}


/******** COMMANDS PARSING AND GESTION  *******/


void	Server::parsingBuffer(int fd)
{
	if (DEBUG)
		std::cout << _clients[fd].getBuffer() << std::endl;
}

//Command Map Functions
void	Server::initCommands(void) {
	_commandMap["PASS"] = &Server::handlePass;
	_commandMap["NICK"] = &Server::handleNick;
	_commandMap["USER"] = &Server::handleUser;
	// _commandMap["PING"] = &Server::handlePing;
	// _commandMap["QUIT"] = &Server::handleQuit;
	// _commandMap["PRIVMSG"] = &Server::handlePrivmsg;
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
	else {
		(void)ERR_UNKNOWNCOMMAND(command);//No match found
		return ;
	}
}
void	Server::handlePass(Client &client, const Message& msg)
{
	(void)client;
	(void)msg;
	std::cout << "Pass ok" << std::endl;
}
void	Server::handleNick(Client &client, const Message& msg)
{
	(void)client;
	(void)msg;
	std::cout << "Nick ok" << std::endl;
}
void	Server::handleUser(Client &client, const Message& msg)
{
	(void)client;
	(void)msg;
	std::cout << "User ok" << std::endl;
}
