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

//Orthodox Canonical Form
Bot::Bot(void) {};
Bot::Bot(const Bot& other) {};//to fix
Bot	&Bot::operator =  (const Bot& other) {
	if (this != &other)
		;//to fix}
	return (*this);
}
Bot::~Bot(void) {};
