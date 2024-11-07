/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/07 15:51:28 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <unordered_set>
# include <chrono>
# include "Utils.hpp"
# include "Server.hpp"
# include "numericReplies.hpp"

using std::string;

class Server;

typedef std::unordered_set<string>	t_StringSet;

class Client
{
	private:
		static t_StringSet	_usedNicknames;
		string				_Nickname;
		string				_Username;
		string				_Hostname;
		string				_sendBuffer;
		string				_recvBuffer;
		bool				_registered = false;
		t_TimeStamp			_programmedDisconnection;
		bool				_isPingSent = true;
		bool				_isPasswordCorrect = false;

		bool				_isNicknameAvailable(string nickname);
		void				_markAsRegistered(Server *server);

	public:
		Client(const Client &other)				= delete;
		Client &operator=(const Client &other)	= delete;
		Client();
		~Client();

		const string		&getNickname() const;
		const string		&getUsername() const;
		const string		&getHostname() const;

		bool				operator==(const Client &other) const;

	// *** Buffer methods ***
		void				addToSendBuffer(string buffer);
		void				addToRecvBuffer(string buffer);
		void				clearSendBuffer();
		void				clearRecvBuffer();
		const string		&getSendBuffer() const;
		const string		&getRecvBuffer() const;

	// *** Registration methods ***
		const bool			&IsPasswordCorrect() const;
		const bool			&IsRegistered() const;
		void				setHostname(string &&Hostname);
		void				setNickname(string nickname, Server *server);
		void				setPasswordCorrect(const bool);
		void				setUsername(string username, Server *server);

	// *** Timeout methods ***
		const t_TimeStamp	&getProgrammedDisconnection() const;
		const t_TimeStamp	&getConnectionTime() const;
		void				pingClient();
		void				resetPingTimer(const string &line);
		void				setProgrammedDisconnection(std::chrono::seconds seconds);
};

#endif
