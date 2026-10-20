/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 14:50:05 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/20 16:44:39 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"


// **** ORTHODOX CANONICAL FORM AND CONSTRUCTORS ****
irc::irc( void ){}

irc::irc(const irc& copy) : _port(copy._port), _pwd(copy._pwd){}

irc &irc::operator=( const irc& copy ) 
{
    if(this != &copy)
    {
        _port = copy._port;
        _pwd = copy._pwd;

    }
    return *this;
}

irc::~irc( void ){}

// **** PARSING ****
int irc::parsingPort(std::string str) const
{
    for(int i(0) ; str[i] ; i++)
    {
        if( !(str[i] >= '0' && str[i] <= '9'))
            return 1;
    }
    if(str.size() > 5 && str.size() < 4)
        return 2;
    long long val = strtoll(str.c_str(), NULL, 10);
    
    if (val < 1024 || val > 65535)
        return 2;
    return val;
    
}

int parsingPwd(std::string str) const
{
    if(str.size() == 0)
        return 1;
}
    

irc::irc( std::string port, std::string pwd)
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
    exec(val, pwd);    
}

