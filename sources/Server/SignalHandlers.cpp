/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandlers.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 15:33:52 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 16:04:31 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/class/Server.hpp"

bool	Server::interruptFlag = false;

void	Server::initSignals(void)
{
	sigIntAct.sa_handler = signal_handler;
	sigemptyset(&sigIntAct.sa_mask);
	sigIntAct.sa_flags = 0;
	sigaction(SIGINT, &sigIntAct, NULL);
}

void	Server::signal_handler(int signum)
{
	(void)signum;
	interruptFlag = false;
}