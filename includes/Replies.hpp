/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 14:21:19 by malaimo           #+#    #+#             */
/*   Updated: 2026/10/30 15:08:47 by malaimo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "Headers.hpp"

// 401 ERR_NOSUCHNICK
# define ERR_NOSUCHNICK(nickName) (":localhost 401 " + nickName + " :No such nick/channel\r\n")

// 402 ERR_NOSUCHSERVER
# define ERR_NOSUCHSERVER(serverName) (":localhost 402 " + serverName + " :No such server\r\n")

// 403     ERR_NOSUCHCHANNEL
# define ERR_NOSUCHCHANNEL(channelName) (":localhost 403 "+  channelName + " :No such channel\r\n")

// 404     ERR_CANNOTSENDTOCHAN
# define ERR_CANNOTSENDTOCHAN(channelName) (":localhost 404 " + channelName + " :Cannot send to channel\r\n")

// 405     ERR_TOOMANYCHANNELS
# define ERR_TOOMANYCHANNELS(channelName) (":localhost 405 " + channelName + " :You have joined too many channels\r\n")

// 406     ERR_WASNOSUCHNICK
# define ERR_WASNOSUCHNICK(nickName) (":localhost 406 " + nickName + " :There was no such nickname\r\n")

// 407     ERR_TOOMANYTARGETS
# define ERR_TOOMANYTARGETS(target) (":localhost 407 " + target + " :Duplicate recipients. No message delivered\r\n")

// 409     ERR_NOORIGIN
# define ERR_NOORIGIN() (":localhost 409 :No origin specified\r\n")

// 411     ERR_NORECIPIENT
# define ERR_NORECIPIENT(command) (":localhost 411 :No recipient given " + command + "\r\n")

// 412     ERR_NOTEXTTOSEND
# define ERR_NOTEXTTOSEND() (":localhost 412 :No text to send\r\n")

// 413     ERR_NOTOPLEVEL
# define ERR_NOTOPLEVEL(command) (":localhost 413 " + mask + " :No toplevel domain specified\r\n")

// 414     ERR_WILDTOPLEVEL
# define ERR_WILDTOPLEVEL(command) (":localhost 414 " + mask + " :Wildcard in toplevel domain\r\n")

// 421     ERR_UNKNOWNCOMMAND
# define ERR_UNKNOWNCOMMAND(command) (":localhost 421 " + command + " :Unknown command\r\n")

// 422     ERR_NOMOTD
# define ERR_NOMOTD() (":localhost 422 :MOTD File is missing\r\n")

// 423     ERR_NOADMININFO
# define ERR_NOSUCHSERVER(server) (":localhost 423 " + server + " :No administrative info available\r\n")

// 424     ERR_FILEERROR
# define ERR_FILEERROR(fileOp, file) (":localhost 424 :File error doing " + fileOp + " on " + file + "\r\n")

// 431     ERR_NONICKNAMEGIVEN
# define ERR_NONICKNAMEGIVEN() (":localhost 431 :No nickname given\r\n")

// 432     ERR_ERRONEUSNICKNAME
# define ERR_ERRONEUSNICKNAME(nick) (":localhost 432 " + nick + " :Erroneus nickname\r\n")

// 433     ERR_NICKNAMEINUSE
# define ERR_NICKNAMEINUSE(nick) (":localhost 433 " + nick + " :Nickname is already in use\r\n")

// 436     ERR_NICKCOLLISION
# define ERR_NICKCOLLISION(nick) (":localhost 436 " + nick + " :Nickname collision KILL\r\n")

// 441     ERR_USERNOTINCHANNEL
# define ERR_USERNOTINCHANNEL(nick, channel) (":localhost 441 " + nick + " " + channel + " :They aren't on that channel\r\n")

// 442     ERR_NOTONCHANNEL
# define ERR_NOTONCHANNEL(channel) (":localhost 442 " + channel + " :You're not on that channel\r\n")

// 443     ERR_USERONCHANNEL
# define ERR_USERONCHANNEL(nick, channel) (":localhost 443 " + nick + " " + channel + " :is already on channel\r\n")

// 444     ERR_NOLOGIN
# define ERR_NOLOGIN(user) (":localhost 444 " + channel + " :User not logged in\r\n")

// 445     ERR_SUMMONDISABLED
# define ERR_SUMMONDISABLED() (":localhost 445 :SUMMON has been disabled\r\n")

// 446     ERR_USERSDISABLED
# define ERR_USERSDISABLED() (":localhost 446 :USERS has been disabled\r\n")

// 451     ERR_NOTREGISTERED
# define ERR_NOTREGISTERED() (":localhost 451 :You have not registered\r\n")

// 461 ERR_NEEDMOREPARAMS
# define ERR_NEEDMOREPARAMS(command) (":localhost 461 " + command + " :Not enough parameters\r\n")

// 462     ERR_ALREADYREGISTRED
# define ERR_ALREADYREGISTRED() (":localhost 462 :You may not reregister\r\n")

// 463     ERR_NOPERMFORHOST
# define ERR_NOPERMFORHOST() (":localhost 463 :Your host isn't among the privileged\r\n")

// 464     ERR_PASSWDMISMATCH
# define ERR_PASSWDMISMATCH() (":localhost 464 :Password incorrect\r\n")

// 465     ERR_YOUREBANNEDCREEP
# define ERR_YOUREBANNEDCREEP() (":localhost 465 :You are banned from this server\r\n")

// 467     ERR_KEYSET
# define ERR_KEYSET(channel) (":localhost 467 " + channel + " :Channel key already set\r\n")

// 471     ERR_CHANNELISFULL
# define ERR_CHANNELISFULL(channel) (":localhost 471 " + channel + " :Cannot join channel (+l)\r\n")

// 472     ERR_UNKNOWNMODE
# define ERR_UNKNOWNMODE(char) (":localhost 472 " + char + ":is unknown mode char to me\r\n")

// 473     ERR_INVITEONLYCHAN
# define ERR_INVITEONLYCHAN(channel) (":localhost 473 " + channel + " :Cannot join channel (+i)\r\n")

// 474     ERR_BANNEDFROMCHAN
# define ERR_BANNEDFROMCHAN(channel) (":localhost 474 " + channel + " :Cannot join channel (+b)\r\n")

// 475     ERR_BADCHANNELKEY
# define ERR_BADCHANNELKEY(channel) (":localhost 475 " + channel + " :Cannot join channel (+k)\r\n")

// 481     ERR_NOPRIVILEGES
# define ERR_NOPRIVILEGES() (":localhost 481 :Permission Denied- You're not an IRC operator\r\n")

// 482     ERR_CHANOPRIVSNEEDED
# define ERR_CHANOPRIVSNEEDED(channel) (":localhost 482 " + channel + " :You're not channel operator\r\n")

// 483     ERR_CANTKILLSERVER
# define ERR_CANTKILLSERVER() (":localhost 483 :You cant kill a server!\r\n")

// 491     ERR_NOOPERHOST
# define ERR_NOOPERHOST() (":localhost 491 :No O-lines for your host\r\n")

// 501     ERR_UMODEUNKNOWNFLAG
# define ERR_UMODEUNKNOWNFLAG() (":localhost 501 :Unknown MODE flag\r\n")

// 502     ERR_USERSDONTMATCH
# define ERR_USERSDONTMATCH() (":localhost 502 :Cant change mode for other users\r\n")


#endif