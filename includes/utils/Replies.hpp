/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 14:21:19 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/12 19:27:51 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#include "Colors.hpp"

//If nickname is empty we will replace it with *
# define GET_NICK(nick) ((nick).empty() ? "*" : (nick))

/******* WELCOME MESSAGE ********/
#define	WELCOME_MESSAGE GREEN <<  "================================================" << std::endl << std::endl \
							<< "================== WELCOME TO ==================" << std::endl << std::endl \
						   << "================= THE WORLD OF =================" << std::endl << std::endl \
					  << PINK "==================== I.R.C =====================" << std::endl << std::endl \
					  <<  "================================================" RESET << std::endl << std::endl 


/******* CHANNELS MESSAGE ********/

# define NEWMEMBERMESSAGE(nickName) ("User " + nickName + " joined the server\r\n")

# define RPL_CHANNELMODEIS(channel, mode, modeParams) (":localhost 324 " + channel + " " + mode + " " + modeParams + " \r\n")

/******* SUCCESSFULL NUMERIC REPLIES ********/
// 331 RPL_NOTOPIC
# define RPL_NOTOPIC(clientNick, chan) (":localhost 331 " + GET_NICK(clientNick) + " " + chan + " :No topic is set\r\n")//a voir

// 332 RPL_TOPIC
# define RPL_TOPIC(clientNick, chan, topic) (":localhost 332 " + GET_NICK(clientNick) + " " + chan + " :" + topic + "\r\n")

/******* ERRORS MESSAGES ********/

// 341 RPL_INVITING
#define RPL_INVITING(nick, invitee, chan)    (std::string(":localhost 341 ") + (nick) + " " + (invitee) + " " + (chan) + "\r\n")

// 401 ERR_NOSUCHNICK
# define ERR_NOSUCHNICK(nick, target) (":localhost 401 " + GET_NICK(nick) + " " + target + " :No such nick/channel\r\n")

// 402 ERR_NOSUCHSERVER
# define ERR_NOSUCHSERVER(serverName) (":localhost 402 " + serverName + " :No such server\r\n")

// 403     ERR_NOSUCHCHANNEL
# define ERR_NOSUCHCHANNEL(nick, chan) (":localhost 403 " + GET_NICK(nick) + " " + target " :No such channel\r\n")

// 404     ERR_CANNOTSENDTOCHAN
# define ERR_CANNOTSENDTOCHAN(nick, chan) (":localhost 404 " + GET_NICK(nick) + " " + chan " :Cannot send to channel\r\n")

// 405     ERR_TOOMANYCHANNELS
# define ERR_TOOMANYCHANNELS(nick, chan) (":localhost 405 " + GET_NICK(nick) + " " + chan " :You have joined too many channels\r\n")

// 406     ERR_WASNOSUCHNICK
# define ERR_WASNOSUCHNICK(nickName) (":localhost 406 " + nickName + " :There was no such nickname\r\n")

// 407     ERR_TOOMANYTARGETS
# define ERR_TOOMANYTARGETS(target) (":localhost 407 " + target + " :Duplicate recipients. No message delivered\r\n")

// 409     ERR_NOORIGIN
# define ERR_NOORIGIN() (":localhost 409 :No origin specified\r\n")

// 411     ERR_NORECIPIENT
# define ERR_NORECIPIENT(nick, command) (":localhost 411 " + GET_NICK(nick) + " :No recipient given " + command + "\r\n")

// 412     ERR_NOTEXTTOSEND
# define ERR_NOTEXTTOSEND(nick) (":localhost 412 " + GET_NICK(nick) " :No text to send\r\n")

// 413     ERR_NOTOPLEVEL
# define ERR_NOTOPLEVEL(command) (":localhost 413 " + mask + " :No toplevel domain specified\r\n")

// 414     ERR_WILDTOPLEVEL
# define ERR_WILDTOPLEVEL(command) (":localhost 414 " + mask + " :Wildcard in toplevel domain\r\n")

// 421     ERR_UNKNOWNCOMMAND
# define ERR_UNKNOWNCOMMAND(nick, command) (":localhost 421 " + GET_NICK(nick) + " " + command + " :Unknown command\r\n")

// 422     ERR_NOMOTD
# define ERR_NOMOTD() (":localhost 422 :MOTD File is missing\r\n")

// 423     ERR_NOADMININFO
# define ERR_NOADMININFO(server) (":localhost 423 " + server + " :No administrative info available\r\n")

// 424     ERR_FILEERROR
# define ERR_FILEERROR(fileOp, file) (":localhost 424 :File error doing " + fileOp + " on " + file + "\r\n")

// 431     ERR_NONICKNAMEGIVEN
# define ERR_NONICKNAMEGIVEN(nick) (":localhost 431 " + GET_NICK(nick) + " :No nickname given\r\n")

// 432     ERR_ERRONEUSNICKNAME
# define ERR_ERRONEUSNICKNAME(nick, badnick) (":localhost 432 " + GET_NICK(nick) + " " + badnick + " :Erroneus nickname\r\n")

// 433     ERR_NICKNAMEINUSE
# define ERR_NICKNAMEINUSE(nick, badnick) (":localhost 433 " + GET_NICK(nick) + " "  + badnick + " :Nickname is already in use\r\n")

// 441     ERR_USERNOTINCHANNEL
# define ERR_USERNOTINCHANNEL(nick, target, chan) (":localhost 441 " + GET_NICK(nick) + " " + target + " " + chan + " :They aren't on that channel\r\n")

// 442     ERR_NOTONCHANNEL
# define ERR_NOTONCHANNEL(nick, chan) (":localhost 442 " + GET_NICK(nick) + " " + chan + " :You're not on that channel\r\n")

// 443     ERR_USERONCHANNEL
# define ERR_USERONCHANNEL(nick, target, chan) (":localhost 443 " + GET_NICK(nick) + " " + target + " " + chan + " :is already on channel\r\n")

// 444     ERR_NOLOGIN
# define ERR_NOLOGIN(user) (":localhost 444 " + channel + " :User not logged in\r\n")

// 445     ERR_SUMMONDISABLED
# define ERR_SUMMONDISABLED() (":localhost 445 :SUMMON has been disabled\r\n")

// 446     ERR_USERSDISABLED
# define ERR_USERSDISABLED() (":localhost 446 :USERS has been disabled\r\n")

// 451     ERR_NOTREGISTERED
# define ERR_NOTREGISTERED(nick) (":localhost 451 " + GET_NICK(nick) + "  :You have not registered\r\n")

// 461	   ERR_NEEDMOREPARAMS
# define ERR_NEEDMOREPARAMS(nick, command) (":localhost 461  " + GET_NICK(nick) + " " + command + " :Not enough parameters\r\n")

// 462     ERR_ALREADYREGISTRED
# define ERR_ALREADYREGISTRED(nick) (":localhost 462 " + GET_NICK(nick) + "  You may not reregister\r\n")

// 463     ERR_NOPERMFORHOST
# define ERR_NOPERMFORHOST() (":localhost 463 Your host isn't among the privileged\r\n")

// 464     ERR_PASSWDMISMATCH
# define ERR_PASSWDMISMATCH(nick) (":localhost 464 " + GET_NICK(nick) + "  Password incorrect\r\n")

// 465     ERR_YOUREBANNEDCREEP
# define ERR_YOUREBANNEDCREEP() (":localhost 465 You are banned from this server\r\n")

// 467     ERR_KEYSET
# define ERR_KEYSET(channel) (":localhost 467 " + channel + " :Channel key already set\r\n")

// 471     ERR_CHANNELISFULL
# define ERR_CHANNELISFULL(nick, chan) (":localhost 471 " + GET_NICK(nick) + " " + chan + " :Cannot join channel (+l)\r\n")

// 472     ERR_UNKNOWNMODE
# define ERR_UNKNOWNMODE(nick, char) (":localhost 472 " + GET_NICK(nick) + " " + char + " :is unknown mode char to me\r\n")

// 473     ERR_INVITEONLYCHAN
# define ERR_INVITEONLYCHAN(nick, chan) (":localhost 473 " + GET_NICK(nick) + " " + chan + " :Cannot join channel (+i)\r\n")

// 474     ERR_BANNEDFROMCHAN
# define ERR_BANNEDFROMCHAN(nick, chan) (":localhost 474 " + GET_NICK(nick) + " " + chan + " :Cannot join channel (+b)\r\n")

// 475     ERR_BADCHANNELKEY
# define ERR_BADCHANNELKEY(nick, chan) ("localhost 475 " + GET_NICK(nick) + " " + chan + " :Cannot join channel (+k)\r\n")

// 481     ERR_NOPRIVILEGES
# define ERR_NOPRIVILEGES() (":localhost 481 Permission Denied- You're not an IRC operator\r\n")

// 482     ERR_CHANOPRIVSNEEDED
# define ERR_CHANOPRIVSNEEDED(nick, chan) (":localhost 482 " + GET_NICK(nick) + " " + chan + " :You're not channel operator\r\n")

// 483     ERR_CANTKILLSERVER
# define ERR_CANTKILLSERVER() (":localhost 483 You cant kill a server!\r\n")

// 491     ERR_NOOPERHOST
# define ERR_NOOPERHOST() (":localhost 491 No O-lines for your host\r\n")

// 501     ERR_UMODEUNKNOWNFLAG
# define ERR_UMODEUNKNOWNFLAG() (":localhost 501 Unknown MODE flag\r\n")

// 502     ERR_USERSDONTMATCH
# define ERR_USERSDONTMATCH() (":localhost 502 Cant change mode for other users\r\n")


#endif
