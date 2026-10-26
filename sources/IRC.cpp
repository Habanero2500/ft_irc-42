/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 14:50:05 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/26 16:31:07 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"
#include <iostream>

//Test
/**** ORTHODOX CANONICAL FORM AND CONSTRUCTORS ****/
irc::irc(void){}

irc::irc(const irc& copy) : _port(copy._port), _pwd(copy._pwd){}

irc &irc::operator=(const irc& copy) 
{
    if(this != &copy)
    {
        _port = copy._port;
        _pwd = copy._pwd;

    }
    return (*this);
}

irc::~irc(void){}

/**** PARSING ****/
int irc::parsingPort(const std::string str) 
{
    for(int i(0) ; str[i] ; i++)
    {
        if(!(str[i] >= '0' && str[i] <= '9'))
            return (1);
    }
    if(str.size() > 5 && str.size() < 4)
        return (2);
    long long val = strtoll(str.c_str(), NULL, 10);
    
    if (val < 1024 || val > 65535)
        return (2);
    return (val);
    
}

int irc::parsingPwd(const std::string str) 
{
    if(str.size() == 0)
        return (1);
    return (0);
}
    

irc::irc(const char* port, const char* pwd)
{
    
    int val = parsingPort(port);
    switch(val)
    {
        case 1 :
            throw notANumberException();
        case 2 :
            throw valueException(); 
    }
    if(parsingPwd(pwd) == 1)
        throw pwdEmptyException();
    
    _port = val;
    _pwd = pwd;
    std::cout << "OK" << std::endl; 
    // exec(val, pwd);   
}


/**** EXCEPTIONS ****/
const char*	irc::pwdEmptyException::what() const throw () {
	return ("The password passed as an argument cannot be empty");
}

const char*	irc::valueException::what() const throw () {
        return ("The port value has to be in the range 1024 to 65535");
}

const char*	irc::notANumberException::what() const throw () {
        return ("Only digits are admitted to choose the port number");
}
