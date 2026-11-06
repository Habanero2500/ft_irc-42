/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:08:39 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/06 16:59:36 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <iostream>
#include <map>
#include <bitset>
#include <vector>

enum modes{
	P,
	S,
	I,
	T,
	N,
	M,
	L,
	K,
};

struct	Member{
	Client	client;
	bool	op;
	bool	voice;

	Member() : op(false), voice(false){}
};

struct	bannedMember{
	std::string 			nickname;
    std::string             username;
    std::string             hostname;

	Member() : op(false), voice(false){}
};

class Channel
{
	private:
	std::string				_name;
	std::map<int, Member>	_members;
	std::vector<int>		_operators;
	std::vector<int>		_banList;
	std::bitset<8>			_modes;
	size_t					_maxMembers;

	public:
	Channel();
	Channel(std::string name);
	Channel(const Channel &copy);
	Channel	&operator=(const Channel &copy);
	~Channel();

	void		setName(std::string name);
	
	std::string	getName();
	size_t		getNumberMember();	
	// Member		getMember();

	void		addMember(Client member);
	void		setOperator(Client member);
	
	bool		isModeActive(int mode);
	bool		isMembersLimitReached();
	bool		isMemberBanned(int fd);
};
	