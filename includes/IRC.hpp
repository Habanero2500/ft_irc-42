/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 12:52:43 by jmetayer          #+#    #+#             */
/*   Updated: 2026/10/20 13:00:33 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <stdexcept> 

class irc {

    public : 

    // **** ORTHODOX CANONICAL FORM ****
    irc(void);
    irc(const irc& copy);
    irc& operator = (const irc& copy);
    ~irc(void);

    //**** USEFUL CONSTRUCTOR ****
    irc( const char* port, const char* pwd);

    //
    

    private : 
    
    uint _port;
    std::string _pwd; 
    
};




#endif