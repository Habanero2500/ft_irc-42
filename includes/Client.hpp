/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:33 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/30 13:49:03 by malaimo          ###   ########.fr       */
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
    void 			setName(std::string name);
    void 			setID(std::string ID);
    void 			setFd(int i);
	void 			setPollFd();
	void 			addBuffer(char buffer[1024]);
    std::string 	getName(void);
    std::string 	getID(void);
    int 			getFd(void);
	struct pollfd 	getPollFd(void); 
	std::string		getBuffer(void);

    private :

    std::string 			_name;
    std::string 			_ID;
    int 					_fd;
	struct sockaddr_in 		_adress;
	struct pollfd			_pollFd;
	std::string				_buffer;
    //std::vector<std::string> _channels;
    
     
};




#endif