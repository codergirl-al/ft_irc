/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericReplies.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:16:12 by ajakob            #+#    #+#             */
/*   Updated: 2024/10/28 17:47:09 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// MULTI USE
#define RPL_WELCOME(client, nick, user, host) (client + " :Welcome to the ft_irc Network, "  + nick + "[!" + user + "@" + host + "]\r\n")
#define ERR_NEEDMOREPARAMS(client, command) (client + " " + command + " :Not enough parameters\r\n")

// USER
#define ERR_ALREADYREGISTRED(client) (client + " :You may not reregister\r\n") 

// PASS
#define ERR_PASSWDMISMATCH(client) (client + " :Password incorrect\r\n")
// #define ERR_YOUREBANNEDCREEP(client) (client + " :You are banned from this server. Creep...")

// NICK
#define ERR_NONICKNAMEGIVEN(client) (client + " :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(client, nick) (client + " " + nick + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\r\n")
// ERR_USERNOTINCHANNEL Maybe?
// ERR_NOTONCHANNEL Maybe?
#define ERR_USERONCHANNEL(client, nick, channel) (client + " " + nick + " " + channel + " :is already on channel\r\n")

// OPER
#define RPL_YOUREOPER(client) (client + " :You are now an IRC operator\r\n")

// MODE
#define ERR_USERSDONTMATCH(client) (client + " :Cant change mode for other users\r\n")
#define RPL_UMODEIS(client, mode) (client + " " + mode + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) (client + " :Unknown MODE flag\r\n")

// CHANNEL
#define ERR_INVITEONLYCHAN(client, channel) (client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel) (client + " " + channel + " :Cannot join channel (+k)\r\n")
// #define ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask")
#define ERR_CHANNELISFULL(client, channel) (client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (client + " " + channel + " :No such channel\r\n")

// TOPIC
#define RPL_TOPIC(client, channel, topic) (client + " " + channel + " :" + topic + "\r\n")
#define RPL_NOTOPIC(client, channel) (client + " " + channel + " :No topic is set\r\n")

// PRIVMSG
// ERR_TOOMANYTARGETS