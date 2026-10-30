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
	return ;
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

//Helper Functions
void	Message::clearParser(void) {
	_parser.prefix.clear();
	_parser.command.clear();
	_parser.params.clear();
	_parser.trailing.clear();
}

void	Message::deleteSpaces(std::string& input) const {
	while (!input.empty() && input[START] == ' ')
		input.erase(START, 1);
}

//CRLF (Carriage return Line Feed) = \r\n
//1)Strip trailing CRLF  ok
//2)Extract trailing first ok
//3)Parse Prefix ok
//4)Parse Command ok
//5)Parse Middle Parameters ok
//6)Append Trailing ok
//Parsing Functions
//PRIVMSG bajuju :Yo yo -> command = PRIVMSG params = 'bajuju' 'yo yo'
void	Message::stripCRLF(std::string& input) const {
	while (!input.empty() && (input[input.size() - 1] == '\r' || input[input.size() - 1] == '\n'))
			input.erase(input.size() - 1);
}

void	Message::extractTrailing(std::string& input) {
	size_t	pos = input.find(" :");
	
	if (pos != std::string::npos) {
		_parser.trailing = input.substr(pos + OFFSET);
		input.erase(pos);
	}
}

//Pas sur si les prefix nous interessent
void	Message::parsePrefix(std::string& input) {
	if (input.empty() || input[START] != ':' )
		return ;
	size_t pos = input.find(' ');
	if (pos != std::string::npos) {
		_parser.prefix = input.substr(1, pos - 1);//No magic numbers
		input.erase(START, pos + 1);
	}
	else { 
		_parser.prefix = input.substr(1); //When only a prefix
		input.clear();
	}
}
//Commands are single words
void	Message::parseCommand(std::string& input) {
	if (input.empty())
		return ;
	deleteSpaces(input);
	size_t	pos = input.find(' ');
	if (pos != std::string::npos) {
		_parser.command = input.substr(START, pos);
		input.erase(START, pos + 1);
	}
	else {
		_parser.command = input.substr(START);
		input.clear();
	}
	for (size_t i = 0; i < _parser.command.size(); ++i) //Case Insensivity
		_parser.command[i] = static_cast<char>(std::toupper(_parser.command[i]));
}

void	Message::parseParams(std::string& input) {
	while (!input.empty()) {
		deleteSpaces(input);
		if (input.empty())
			break ;

		size_t	pos = input.find(' ');
		if (pos != std::string::npos) {
			_parser.params.push_back(input.substr(START, pos));
			input.erase(START, pos + 1);
		}
		else {
			_parser.params.push_back(input.substr(START));
			input.clear();
		}
	}
	if (!_parser.trailing.empty()) //We now add the trailing element
		_parser.params.push_back(_parser.trailing);
}

void	Message::parsing(std::string& input) {
	if (input.empty())
		return ;

	stripCRLF(input);
	if (input.empty())
		return ;

	extractTrailing(input);
	parsePrefix(input);
	parseCommand(input);
	parseParams(input);
}
