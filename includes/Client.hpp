/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:33 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/03 11:29:04 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"

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
	void 			setPollFd();
	void 			addBuffer(char buffer[1024]);
    std::string 	getName(void);
    std::string 	getID(void);
    int 			getFd(void);
	struct pollfd 	getPollFd(void); 
	std::string		getBuffer(void);
    std::string 	getNickname(void); 
    std::string 	getUsername(void);
    std::string 	getHostname(void);
    std::string 	getRealName(void);
    
    private :

    std::string 			_nickname;
    std::string             _username;
    std::string             _hostname;
    std::string             _realname;
    
    std::string 			_ID;
    int 					_fd;
	struct sockaddr_in 		_adress;
	struct pollfd			_pollFd;
	std::string				_buffer;

    bool                    _pass;
    bool                    _nick;
    bool                    _user;
    
    
     
};




#endif