/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opapouts <opapouts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:08:39 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 19:34:32 by opapouts         ###   ########.fr       */
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
	Client	*client;
	bool	op;
	bool	voice;
};

struct	BannedMember{
	std::string 			nickname;
	std::string             username;
	std::string             hostname;

	BannedMember() : nickname(""), username(""), hostname(""){}
};


class Channel
{
	public:
		Channel();
		Channel(std::string name);
		Channel(const Channel &copy);
		Channel	&operator=(const Channel &copy);
		~Channel();

		std::string	getTopic(void) const;
		std::string	getName() const;
		size_t		getNumberMember() const;
		Member*		getMemberByNick(const std::string& user);
		std::string	getModes() const;
		size_t		getMaxMember() const;
		std::string	getPwd() const;
	
		void		setName(std::string &name);
		void		setTopic(const std::string& topicName);
		void		setPwd(std::string &pwd);
		void		setOperator(Client &member);

		void		addMember(Client &member);
	
		bool		isModeActive(int mode) const;
		bool		isTopicSet(void) const;
		bool		isMembersLimitReached() const;
		bool		isClientAnOperator(const Client& client) const;
		bool		isMemberBanned(Client &member);
		bool		isMember(Client &member);
		bool		isOperator(Client &member);//a effacer


		//Broadcast and shit
		void	broadcast(Server& server, const std::string& msg) const;
		void	removeClientFromChannel(Client& client);

	private:
		std::string					_name;
		std::map<int, Member>		_members;
		std::vector<int>			_chouffins;
		std::vector<BannedMember>	_banList;
		std::bitset<8>				_modes;
		size_t						_maxMembers;
		std::string					_pwd;
		std::string					_topic;

};

#endif
