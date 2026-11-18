/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 15:08:39 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 20:03:18 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <map>
#include <bitset>
#include <vector>
#include <set>

//Forwards declarations
class	Server;

enum modes{
	I,
	T,
	L,
	K,
};

struct	Member{
	Client	*client;
	bool	op;
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
		std::string	getNamesList() const;
		std::string getModesArg() const;

		void		setName(std::string &name);
		void		setTopic(const std::string& topicName);
		void		setPwd(const std::string &pwd);
		void		setmode(const int &mode);
		void		removemode(const int &mode);
		void		setMaxMembers(std::string max);

		void		addMember(Client &member);
		bool		setOperator(Client &member);
		bool		removeOperator(Client &member);
	
		bool		isModeActive(int mode) const;
		bool		isTopicSet(void) const;
		bool		isMembersLimitReached() const;
		bool		isClientAnOperator(const Client& client) const;
		bool		isEmpty(void);


		bool		isMember(const Client &member) const;
		bool		isPwdCorrect(const std::string &pwd) const;

		//Broadcast and shit
		void	broadcast(Server& server, const std::string& msg) const;
		void	broadcastQuit(Server& server, Client& client, const std::string& msg, std::set<int>& notifiedFds) const;
		void	broadcastPrivmsg(Server& server, Client& client, const std::string& msg) const;
		void	removeClientFromChannel(Client& client);

		std::string	size_tToString(size_t src) const;
		
	private:
		std::string					_name;
		std::map<int, Member>		_members;
		std::bitset<8>				_modes;
		size_t						_maxMembers;
		std::string					_pwd;
		std::string					_topic;

};
	
#endif
