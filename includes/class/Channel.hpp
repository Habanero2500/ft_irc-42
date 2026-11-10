/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:08:39 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/10 17:55:12 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <iostream>
#include <map>
#include <bitset>
#include <vector>
#include <set>

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

struct	BannedMember{
	std::string 			nickname;
    std::string             username;
    std::string             hostname;

	BannedMember() : nickname(""), username(""), hostname(""){}
};


class Channel
{
	private:
	std::string					_name;
	std::map<int, Member>		_members;
	std::vector<int>			_operators;
	std::vector<BannedMember>	_banList;
	std::bitset<8>				_modes;
	size_t						_maxMembers;
	std::set<std::string>		_invited; // pasde doublon et on y accede par valeur (on itere pas)

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
	bool		isMemberBanned(Client Member);
	bool	isMember(int fd);
	bool	isOperator(int fd);
	void	addInvite(const std::string &nick);
	bool	isInvited(const std::string &nick);
	void	removeInvite(const std::string &nick);
};
	