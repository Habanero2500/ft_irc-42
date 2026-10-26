/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:33 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 17:01:08 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;
class Client{
  
    public :
    

    // **** ORTHODOX CANONICAL FORM ****
    Client (void);
    ~Client (void);
    Client(const Client& copy);
    Client& operator=(const Client& copy);

	void	initClientAdress(int port);

    //**** SETTERS AND GETTERS ****
    // void setName(std::string name);
    // void setID(std::string ID);
    // void setFd(int i);
    // std::string getName(void);
    // std::string getID(void);
    // int getFd(void); 

    private :

    std::string 			_name;
    std::string 			_ID;
    int 					_fd;
	struct sockaddr_in 		_adress;
    //std::vector<std::string> _channels;
    
     
    

    
};




#endif