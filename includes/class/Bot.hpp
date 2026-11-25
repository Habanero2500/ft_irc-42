/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 20:54:22 by opapouts          #+#    #+#             */
/*   Updated: 2026/11/23 20:54:24 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#define BOT_HOST "127.0.0.1"
#define BOT_PORT "6667"
#define BOT_PWD "code"
#include <string>

class	Bot
{
	public :
		Bot(const char* portStr, const char* pwd, const char* nickname);
		~Bot(void);

		void	run();

	private :

		//Orthodox Canonical Form
		Bot(void);
		Bot(const Bot& other);
		Bot &operator = (const Bot& other);

		void	connectToServer(void);
		void	authenticate(void);
		void	sendRaw(const std::string& msg) const;
		void	handleServerData(void);
		void	handleStdinData(void);
		void	processLine(std::string& line);

		int		_fd;
		int		_port;
		std::string 	_pwd;
		std::string 	_nickname;
		std::string 	_recvBuffer;
		bool		_running;


};


#endif
