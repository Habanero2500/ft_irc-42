/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 14:50:05 by jmetayer          #+#    #+#             */
/*   Updated: 2026/11/24 14:32:16 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/class/IRC.hpp"
#include <cstdlib>

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
int irc::parsingPort(const std::string& str) 
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


int irc::getPort(void) const{return _port;}
std::string irc::getPwd() const{return _pwd;}

int irc::parsingPwd(const std::string& str) 
{
    if (str.empty())
        return (1);
    if (str.find_first_of(" ") != std::string::npos)
        return (2);
    return (0);
}
static std::string stripPwd(const std::string &s)
{
    std::string res;

    for (long unsigned int i(0); i < s.size(); i++)
    {       
        if (std::string("\a\b\t\n\v\f\r").find(s[i]) == std::string::npos)
            res += s[i];
    }
    return res;
}
irc::irc(const char* port, const char* pwd)
{
    switch(parsingPort(port))
    {
        case 1 :
            throw notANumberException();
        case 2 :
            throw valueException(); 
    }
    switch(parsingPwd(pwd))
    {
        case 1 : 
            throw pwdEmptyException();
        case 2 : 
            throw spaceCharacterException();
    }
    _port = parsingPort(port);
    _pwd = stripPwd(pwd);   
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

const char*	irc::spaceCharacterException::what() const throw () {
        return ("Spaces cannot be used to set the password");
}