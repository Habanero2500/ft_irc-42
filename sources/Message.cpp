/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 16:55:12 by opapouts          #+#    #+#             */
/*   Updated: 2026/10/26 16:55:13 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Message.hpp"

//Orthodox Canonical Form
Message::Message(void) {
	_parser.prefix.clear();
	_parser.command.clear();
	_parser.params.clear();
	_parser.trailing.clear();
}

Message::Message(const Message& other) {
	_parser.prefix = other._parser.prefix;
	_parser.command = other._parser.command;
	_parser.params = other._parser.params;
	_parser.trailing = other._parser.trailing;
}

Message &Message::operator = (const Message& other) {
	if (this != &other) {
		_parser.prefix = other._parser.prefix;
		_parser.command = other._parser.command;
		_parser.params = other._parser.params;
		_parser.trailing = other._parser.trailing;
	}
	return (*this);
}

Message::~Message(void) {
	return ;
}
//CRLF (Carriage return Line Feed) = \r\n
//1)Strip trailing CRLF  ok
//2)Extract trailing first ok
//3)Parse Prefix ok?
//4)Parse Command
//5)Parse Middle Parameters
//6)Append Trailing
//Parsing Functions
//PRIVMSG bajuju :Yo yo -> command = PRIVMSG params = 'bajuju' 'yo yo'
bool	Message::stripCRLF(const std::string& input) const {
	size_t	len = input.size();
	if (len < CRLF)
		return (false);
	else if (input[len - 1] == '\n' && input[len - 2] == '\r')
			return (true);
	return (false);
}

std::string	Message::extractTrailing(std::string& input) {
	size_t	pos = input.find(" :");
	
	if (pos != std::string::npos) {
		_parser.trailing = input.substr(pos + OFFSET);
		input.erase(pos);
	}
		return (input);
}

//Pas sur si les prefix nous interessent
void	Message::parsePrefix(std::string& str) {
	if (str[START] != ':')
		return ;
	size_t pos = str.find(' ');//Edge case a traiter
	if (pos != std::string::npos) {
		_parser.prefix = str.substr(START, pos + 1);
		str.erase(START, pos);
	}
}

void	Message::parseCommand(std::string& str) {
}

void	Message::parsing(std::string& input) {
	if (input.empty())
		return ;//To fix

	if (stripCRLF(input))
		input.erase(input.size() - CRLF);
	std::string	str = extractTrailing(input);
	parsePrefix(str);
}
