/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 20:53:57 by opapouts          #+#    #+#             */
/*   Updated: 2026/11/23 20:53:58 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Bot.hpp"
#include "../../includes/class/Message.hpp"
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>


//Orthodox Canonical Form
Bot::Bot(void) {};
Bot::Bot(const Bot& other) : _fd(other._fd), _port(other._port), _pwd(other._pwd), _nickname(other._nickname), _recvBuffer(other._recvBuffer), _running(other._running){
	return ;
}
	
Bot	&Bot::operator =  (const Bot& other) {
	if (this != &other) {
		_fd = other._fd;
		_port = other._port;
		_pwd = other._pwd;
		_nickname = other._nickname;
		_recvBuffer = other._recvBuffer;
		_running = other._running;
	}
	return (*this);
}

Bot::~Bot(void) {
	if (_fd != -1)
		close(_fd);
}

Bot::Bot(const char* portStr, const char* pwd, const char* nickname) : _fd(-1), _pwd(pwd), _nickname(nickname), _running(true) {
	char* end = NULL;
	long port = std::strtol(portStr, &end, 10);

	if (!portStr[0] || *end != '\0' || port <= 0 || port > 65535)
		throw std::runtime_error("BOT: Invalid port");

	if (_pwd.empty() || _nickname.empty())
		throw std::runtime_error("BOT: Empty password or nickname");
	_port = static_cast<int>(port);
}

void	Bot::connectToServer() {
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw std::runtime_error("BOT: Socket creation failed");

	struct sockaddr_in servAddr;
	std::memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);

	if (inet_pton(AF_INET, BOT_HOST, &servAddr.sin_addr) <= 0)
		throw std::runtime_error("BOT: Invalid host address");
	
	if (connect(_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1)
		throw std::runtime_error("BOT: Connection failed (is ircserv running?)");
}

void	Bot::sendRaw(const std::string& msg) const {
	send(_fd, msg.c_str(), msg.size(), 0);
}

void	Bot::authenticate() {
	sendRaw("PASS " + _pwd + "\r\n");
	sendRaw("NICK " + _nickname + "\r\n");
	sendRaw("USER " + _nickname + " 0 * :" + _nickname + "\r\n");
	std::cout << "[BOT] Connected and authenticated as " << _nickname << std::endl;
	std::cout << "[BOT] Type IRC commands below (e.g., JOIN #42):" << std::endl;
}

void	Bot::run() {
	connectToServer();
	authenticate();

	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	fds[1].fd = _fd;
	fds[1].events = POLLIN;
	fds[1].revents = 0;

	while (_running) {
		if (poll(fds, 2, -1) == -1)
			throw std::runtime_error("BOT: Poll failure");

		if (fds[0].revents & POLLIN)
 			handleStdinData();
		if (fds[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
		std::cout << "[BOT] Server closed connection." << std::endl;
		break;
	}
	if (fds[1].revents & POLLIN)
		handleServerData();
	}
}

void Bot::handleStdinData() {
    std::string input;
    if (!std::getline(std::cin, input)) {
        sendRaw("QUIT :Bot shutting down\r\n");
        _running = false;
        return;
    }
    if (!input.empty())
        sendRaw(input + "\r\n");
}

void Bot::handleServerData() {
	char buffer[1024] = {0};
	ssize_t bytes = recv(_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0) {
		std::cout << "[BOT] Disconnected from server." << std::endl;
		_running = false;
		return;
	}
	_recvBuffer += buffer;

	size_t pos;
	while ((pos = _recvBuffer.find('\n')) != std::string::npos) {

		std::string line = _recvBuffer.substr(0, pos);
		_recvBuffer.erase(0, pos + 1);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		if (!line.empty()) {
			std::cout << "<- " << line << std::endl; // Prints every server response
			processLine(line);
		}
	}
}

void Bot::processLine(std::string& line) {
	Message msg;
	msg.parsing(line);
	const Parser& p = msg.getParser();

	if (p.command == "PING" && !p.params.empty())
		sendRaw("PONG :" + p.params[0] + "\r\n");
}
