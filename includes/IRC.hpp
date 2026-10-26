/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:52:43 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 16:43:17 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>

class irc {

    public	: 

    // **** ORTHODOX CANONICAL FORM ****
    irc(void);
    irc(const irc& copy);
    irc& operator = (const irc& copy);
    ~irc(void);

    // **** USEFUL CONSTRUCTOR ****
   irc(const char* port, const char* pwd);

    // **** PARSING ****
    int parsingPort(const std::string str) ;
    int parsingPwd(const std::string str) ;

    //**** EXEC ****
    void exec(int val, std::string pwd);
    
    //****EXCEPTIONS ****
    class	pwdEmptyException : public std::exception { 
		public	: 
			virtual const char *what() const throw();
    };
    class	valueException : public std::exception {
		public : 
			virtual const char *what() const throw();
    };

    class	notANumberException : public std::exception {
		public : 
			virtual const char *what() const throw();
    };

    private	: 
    
    int _port;
    std::string _pwd; 
    
};

#endif
