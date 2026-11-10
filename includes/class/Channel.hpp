/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:08:39 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/06 17:47:34 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <map>
#include <bitset>
#include <vector>

//Forwards declarations
class	Server;

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

	public:
	Channel();
	Channel(std::string name);
	Channel(const Channel &copy);
	Channel	&operator=(const Channel &copy);
	~Channel();

	void		setName(std::string name);
	
	std::string	getName();
	size_t		getNumberMember() const;	
	Member*		getMemberByNick(const std::string& user);

	void		addMember(Client member);
	void		setOperator(Client member);
	
	bool		isModeActive(int mode);
	bool		isMembersLimitReached();
	bool		isMemberBanned(Client Member);

	//Broadcast and shit
	void	broadcast(Server& server, const std::string& msg) const;
	void	removeClientFromChannel(Client& client);

};
	
#endif
