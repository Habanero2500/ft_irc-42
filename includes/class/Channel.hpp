/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:08:39 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/06 13:23:51 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <iostream>
#include <map>
#include <bitset>

class Channel
{
	private:
	std::string				_name;
	std::map<int, Client>	_members;
	std::map<char, bool>	_modes;

	public:
	Channel();
	Channel(std::string name);
	Channel(const Channel &copy);
	Channel	&operator=(const Channel &copy);
	~Channel();

	std::string	getName();
	void		setName(std::string name);
	void		addMember(Client member);
};
	