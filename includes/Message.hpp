/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 16:54:51 by opapouts          #+#    #+#             */
/*   Updated: 2026/10/26 16:54:53 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#define START 0//Pas sur de ces defines
#define	CRLF 2
#define	OFFSET 2
#include <string>
#include <vector>

//[:prefix] <command> <param1> <param2> ... [:trailing] \r\n
struct	Parser {
	std::string			prefix;
	std::string			command;
	std::vector<std::string>	params;
	std::string			trailing;
};

class	Message {

	public	:
		//Orthodox Canonical Form
		Message(void);
		Message(const Message& other);
		Message& operator = (const Message& other);
		~Message(void);

		//Exception Classes

		//Parsing Functions
		void		parsing(std::string& input);
		bool		stripCRLF(const std::string& input) const;
		std::string	extractTrailing(std::string& input);
		void		parsePrefix(std::string& str);
		void		parseCommand(std::string& str);
		//Dispatch Functions
	private	:
		Parser	_parser;


};

#endif
