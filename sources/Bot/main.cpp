/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 20:19:09 by opapouts          #+#    #+#             */
/*   Updated: 2026/11/24 20:19:10 by opapouts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/class/Bot.hpp"
#include <stdlib.h>
#include <iostream>

int main(int ac, char **av)
{
	try {
		if (ac == 2) {
			Bot bot(BOT_PORT, BOT_PWD, av[1]);
			bot.run();
		}
		else if (ac == 4) {
			Bot bot(av[1], av[2], av[3]);
			bot.run();
		}
		else {
			std::cerr << "Usage: ./ircbot <nickname> OR ./ircbot <port> <password> <nickname>" << std::endl;
			return (1);
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
