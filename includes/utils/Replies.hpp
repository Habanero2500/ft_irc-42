/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 14:21:19 by malaimo           #+#    #+#             */
/*   Updated: 2026/11/18 20:03:39 by jmetayer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#include "Colors.hpp"

/******* WELCOME MESSAGE ********/
#define	WELCOME_MESSAGE GREEN <<  "================================================" << std::endl << std::endl \
							<< "================== WELCOME TO ==================" << std::endl << std::endl \
						   << "================= THE WORLD OF =================" << std::endl << std::endl \
					  << PINK "==================== I.R.C =====================" << std::endl << std::endl \
					  <<  "================================================" RESET << std::endl << std::endl 


/******* COMMANDS MESSAGE ********/

#define NEWMEMBERMESSAGE(nickName) ("User " + nickName + " joined the server\r\n")

#define MODESMODIFICATIONMESSAGE(client, channel, modes, arg) (client.nick() + "!" + client.getUsername() + "@" + client.getHostname() + " MODE " + channel + " " + modes + " " + arg + "\r\n" )

// 324 RPL_CHANNELMODEIS
#define RPL_CHANNELMODEIS(client, channel, mode, modeParams) (":localhost 324 " + client.nick() + " " + channel + " " + mode + " " + modeParams + "\r\n")

// 341 RPL_INVITING
#define RPL_INVITING(nick, invitee, chan)    (std::string(":localhost 341 ") + (nick) + " " + (invitee) + " " + (chan) + "\r\n")
					  
/******* SUCCESSFULL NUMERIC REPLIES ********/
// 331 RPL_NOTOPIC
# define RPL_NOTOPIC(client, chan) (":localhost 331 " + (client).getNickname() + " " + chan + " :No topic is set\r\n")

// 332 RPL_TOPIC
# define RPL_TOPIC(client, chan, topic) (":localhost 332 " + (client).getNickname() + " " + chan + " :" + topic + "\r\n")

// 353 RPL_NAMREPLY
# define RPL_NAMREPLY(client, chan, names)  (":localhost 353 " + (client).getNickname() + " = " + (chan) + " :" + (names) + "\r\n")

// 366 RPL_ENDOFNAMES
# define RPL_ENDOFNAMES(client, chan)       (":localhost 366 " + (client).getNickname() + " " + (chan) + " :End of /NAMES list.\r\n")
/******* ERRORS MESSAGES ********/

// 401 ERR_NOSUCHNICK
# define ERR_NOSUCHNICK(client, target) (":localhost 401 " + client.nick() + " " + target + " :No such nick/channel\r\n")

// 402 ERR_NOSUCHSERVER
# define ERR_NOSUCHSERVER(serverName) (":localhost 402 " + serverName + " :No such server\r\n")

// 403     ERR_NOSUCHCHANNEL
# define ERR_NOSUCHCHANNEL(client, chan) (":localhost 403 " + client.nick() + " " + chan + " :No such channel\r\n")

// 404     ERR_CANNOTSENDTOCHAN
# define ERR_CANNOTSENDTOCHAN(client, chan) (":localhost 404 " + client.nick() + " " + chan + " :Cannot send to channel\r\n")

// 405     ERR_TOOMANYCHANNELS
# define ERR_TOOMANYCHANNELS(client, chan) (":localhost 405 " + client.nick() + " " + chan + " :You have joined too many channels\r\n")

// 406     ERR_WASNOSUCHNICK
# define ERR_WASNOSUCHNICK(nickName) (":localhost 406 " + nickName + " :There was no such nickname\r\n")

// 407     ERR_TOOMANYTARGETS
# define ERR_TOOMANYTARGETS(target) (":localhost 407 " + target + " :Duplicate recipients. No message delivered\r\n")

// 409     ERR_NOORIGIN
# define ERR_NOORIGIN() (":localhost 409 :No origin specified\r\n")

// 411     ERR_NORECIPIENT
# define ERR_NORECIPIENT(client, command) (":localhost 411 " + client.nick() + " :No recipient given " + command + "\r\n")

// 412     ERR_NOTEXTTOSEND
# define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client.nick() + " :No text to send\r\n")

// 413     ERR_NOTOPLEVEL
# define ERR_NOTOPLEVEL(command) (":localhost 413 " + mask + " :No toplevel domain specified\r\n")

// 414     ERR_WILDTOPLEVEL
# define ERR_WILDTOPLEVEL(command) (":localhost 414 " + mask + " :Wildcard in toplevel domain\r\n")

// 421     ERR_UNKNOWNCOMMAND
# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client.nick() + " " + command + " :Unknown command\r\n")

// 422     ERR_NOMOTD
# define ERR_NOMOTD() (":localhost 422 :MOTD File is missing\r\n")

// 423     ERR_NOADMININFO
# define ERR_NOADMININFO(server) (":localhost 423 " + server + " :No administrative info available\r\n")

// 424     ERR_FILEERROR
# define ERR_FILEERROR(fileOp, file) (":localhost 424 :File error doing " + fileOp + " on " + file + "\r\n")

// 431     ERR_NONICKNAMEGIVEN
# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client.nick() + " :No nickname given\r\n")

// 432     ERR_ERRONEUSNICKNAME
# define ERR_ERRONEUSNICKNAME(client, badnick) (":localhost 432 " + client.nick() + " " + badnick + " :Erroneus nickname\r\n")

// 433     ERR_NICKNAMEINUSE
# define ERR_NICKNAMEINUSE(client, badnick) (":localhost 433 " + client.nick() + " "  + badnick + " :Nickname is already in use\r\n")

// 441     ERR_USERNOTINCHANNEL
# define ERR_USERNOTINCHANNEL(client, target, chan) (":localhost 441 " + client.nick() + " " + target + " " + chan + " :They aren't on that channel\r\n")

// 442     ERR_NOTONCHANNEL
# define ERR_NOTONCHANNEL(client, chan) (":localhost 442 " + client.nick() + " " + chan + " :You're not on that channel\r\n")

// 443     ERR_USERONCHANNEL
# define ERR_USERONCHANNEL(client, target, chan) (":localhost 443 " + client.nick() + " " + target + " " + chan + " :is already on channel\r\n")

// 444     ERR_NOLOGIN
# define ERR_NOLOGIN(user) (":localhost 444 " + channel + " :User not logged in\r\n")

// 445     ERR_SUMMONDISABLED
# define ERR_SUMMONDISABLED() (":localhost 445 :SUMMON has been disabled\r\n")

// 446     ERR_USERSDISABLED
# define ERR_USERSDISABLED() (":localhost 446 :USERS has been disabled\r\n")

// 451     ERR_NOTREGISTERED
# define ERR_NOTREGISTERED(client) (":localhost 451 " + client.nick() + "  :You have not registered\r\n")

// 461	   ERR_NEEDMOREPARAMS
# define ERR_NEEDMOREPARAMS(client, command) (":localhost 461  " + client.nick() + " " + command + " :Not enough parameters\r\n")

// 462     ERR_ALREADYREGISTRED
# define ERR_ALREADYREGISTRED(client) (":localhost 462 " + client.nick() + "  You may not reregister\r\n")

// 463     ERR_NOPERMFORHOST
# define ERR_NOPERMFORHOST() (":localhost 463 Your host isn't among the privileged\r\n")

// 464     ERR_PASSWDMISMATCH
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client.nick() + "  Password incorrect\r\n")

// 465     ERR_YOUREBANNEDCREEP
# define ERR_YOUREBANNEDCREEP() (":localhost 465 You are banned from this server\r\n")

// 467     ERR_KEYSET
# define ERR_KEYSET(client, chan) (":localhost 467 " + client.nick() + " " + chan + " :Channel key already set\r\n")

// 471     ERR_CHANNELISFULL
# define ERR_CHANNELISFULL(client, chan) (":localhost 471 " + client.nick() + " " + chan + " :Cannot join channel (+l)\r\n")

// 472     ERR_UNKNOWNMODE
# define ERR_UNKNOWNMODE(client, character) (":localhost 472 " + client.nick() + " " + character + " :is unknown mode char to me\r\n")

// 473     ERR_INVITEONLYCHAN
# define ERR_INVITEONLYCHAN(client, chan) (":localhost 473 " + client.nick() + " " + chan + " :Cannot join channel (+i)\r\n")

// 474     ERR_BANNEDFROMCHAN
# define ERR_BANNEDFROMCHAN(client, chan) (":localhost 474 " + client.nick() + " " + chan + " :Cannot join channel (+b)\r\n")

// 475     ERR_BADCHANNELKEY
# define ERR_BADCHANNELKEY(client, chan) (":localhost 475 " + client.nick() + " " + chan + " :Cannot join channel (+k)\r\n")

// 481     ERR_NOPRIVILEGES
# define ERR_NOPRIVILEGES() (":localhost 481 Permission Denied- You're not an IRC operator\r\n")

// 482     ERR_CHANOPRIVSNEEDED
# define ERR_CHANOPRIVSNEEDED(client, chan) (":localhost 482 " + client.nick() + " " + chan + " :You're not channel operator\r\n")

// 483     ERR_CANTKILLSERVER
# define ERR_CANTKILLSERVER() (":localhost 483 You cant kill a server!\r\n")

// 491     ERR_NOOPERHOST
# define ERR_NOOPERHOST() (":localhost 491 No O-lines for your host\r\n")

// 501     ERR_UMODEUNKNOWNFLAG
# define ERR_UMODEUNKNOWNFLAG() (":localhost 501 Unknown MODE flag\r\n")

// 502     ERR_USERSDONTMATCH
# define ERR_USERSDONTMATCH() (":localhost 502 Cant change mode for other users\r\n")


#endif
