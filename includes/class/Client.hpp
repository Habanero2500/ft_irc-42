/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:33 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/09 17:50:45 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>
#include <netinet/in.h>
#include <iostream>

extern bool DEBUG;

class Client{
  
    public :

    // **** ORTHODOX CANONICAL FORM ****
	Client (void);
	~Client (void);
	Client(const Client& copy);
	Client& operator=(const Client& copy);

	void	initClientAdress(int port);

    //**** SETTERS AND GETTERS ****
    void 			setNickname(std::string name);
    void 			setUsername(std::string name);
    void 			setHostname(std::string name);
    void 			setRealName(std::string name);
    void 			setID(std::string ID);
    void 			setFd(int i);
	void 			setPollFd(void);
    void            setPass(void);
    void            setNick(void);
    void            setUser(void);
	void    		setBuffer(std::string Newbuffer);
    void    		setServername(std::string name);
	void 			addBuffer(char buffer[1024]);
    std::string 	getServername(void);
	std::string 	getName(void);
	std::string 	getID(void);
	int 			getFd(void);
	struct pollfd 	getPollFd(void); 
	std::string		getBuffer(void);
    std::string 	getNickname(void) const; 
    std::string 	getUsername(void);
    std::string 	getHostname(void);
    std::string 	getRealName(void);
    int             getAttempt(void);
    bool            getPass(void);
    bool            getNick(void);
    bool            getUser(void);
	int             getNumberChannels(void);

    //**** OTHERS ****
    
    void            increaseAttempt(void);
    bool	        isRegistered(void);
    void	        setRegistered(void);
    bool	        shouldDisconnect(void);
    void	        setToDisconnect(void);
    void            displayClient(void);

	/****** CHANNELS *********/
	void 			increaseNumberChannels(void);
    
	//Send Buffer Management
	void			appendToSendBuffer(const std::string& data);
	std::string&		getSendBuffer(void);
	bool			isSendBufferEmpty(void) const;

    private :

	std::string 			_nickname;
	std::string				_username;
	std::string				_hostname;
	std::string				_serverName;    
	std::string				_realname;
    
	std::string				_ID;
	int 					_fd;
	struct sockaddr_in 		_adress;
	struct pollfd			_pollFd;
	std::string				_buffer;

    bool                    _pass;
    bool                    _nick;
    bool                    _user;
    int                     _attempt;
    
    bool	                _registered;
    bool	                _toDisconnect;

	int						_channelsJoined;
	//Buffer related fields
	std::string				_sendBuffer;//Outgoing data that couldn't be send immediately
};

#endif
