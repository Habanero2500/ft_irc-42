/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:33 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 14:41:02 by jmetayer         ###   ########.fr       */
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
    std::string getName(void);
    std::string getID(void);

    private :

    std::string _name;
    std::string _ID;
    //std::vector<std::string> _channels;
    
     
    

    
};




#endif