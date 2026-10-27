/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:33 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/27 15:02:21 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <algorithm>
#include <poll.h>
#include <fcntl.h>


# define RESET "\e[0m"
# define RED "\e[31m"
# define GREEN "\e[32m"
# define BLUE "\e[1;36m"
# define CYAN "\e[0;36m"
# define PURPLE "\e[0;35m"


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
    void setName(std::string name);
    void setID(std::string ID);
    void setFd(int i);
	void setPollFd();
	void addBuffer(char buffer[1024]);
    std::string getName(void);
    std::string getID(void);
    int getFd(void);
	struct pollfd getPollFd(void); 

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