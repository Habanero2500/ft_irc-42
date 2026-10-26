/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:33 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 16:05:08 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <string>

class Client{
  
    public :
    

    // **** ORTHODOX CANONICAL FORM ****
    Client (void);
    ~Client (void);
    Client(const Client& copy);
    Client& operator=(const Client& copy);

    //**** SETTERS AND GETTERS ****
    void setName(std::string name);
    void setID(std::string ID);
    void setFd(int i);
    std::string getName(void);
    std::string getID(void);
    int getFd(void); 

    private :

    std::string _name;
    std::string _ID;
    int _fd;
    //std::vector<std::string> _channels;
    
     
    

    
};




#endif