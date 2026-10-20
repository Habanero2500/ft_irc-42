/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:52:43 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/20 16:44:52 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <stdexcept> 
#include <string> 

class irc {

    public : 

    // **** ORTHODOX CANONICAL FORM ****
    irc(void);
    irc(const irc& copy);
    irc& operator = (const irc& copy);
    ~irc(void);

    // **** USEFUL CONSTRUCTOR ****
    irc( std::string port, std::string pwd);

    // **** PARSING ****
    int parsingPort(std::string str) const;
    int parsingPwd(std::string str) const;

    //**** EXEC ****
    void exec(int val, std::string pwd);
    
    //****EXCEPTION ****
    class pwdEmptyException : public std::exception{
    
    public : 
    virtual const char *what() const throw()
    {
        return "The password passed as an argument cannot be empty";
    }
    
    };
    
    class valueException : public std::exception{
    
    public : 
    virtual const char *what() const throw()
    {
        return "The port value has to be in the range 1024 to 65535";
    }
    
    };

    class notANumberException : public std::exception{
    
    public : 
    virtual const char *what() const throw()
    {
        return "Only digits are admitted to choose the port number";
    }
    
    };

    private : 
    
    uint _port;
    std::string _pwd; 
    
};




#endif