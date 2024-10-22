/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeoutMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:45:38 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/22 17:58:43 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

const t_TimeStamp	&Client::getConnectionTime() const
{
	return (_connectionTime);
}

void Client::setProgrammedDisconnection(std::chrono::seconds seconds)
{
	_programmedDisconnection = std::chrono::system_clock::now() + seconds;
}

const t_TimeStamp	&Client::getProgrammedDisconnection() const
{
	return (_programmedDisconnection);
}

void Client::pingClient()
{
	if (_isPingSent == true)
		return ;
	string ping = "PING :" + _Nickname + "\r\n";
	addToSendBuffer(ping);
	_isPingSent = true;
}

void Client::resetPingTimer(const string &line)
{
	std::size_t	pos = line.find_first_of(" ");
	string		pong;

	if (pos == std::string::npos)
		return ;
	if (line.find_first_of(":") > pos)
		pos = line.find_first_of(":");
	pos++;
	pong = line.substr(pos);
	if (pong != _Nickname + "\r")
		return ;
	_programmedDisconnection = std::chrono::system_clock::now() + std::chrono::seconds(300);
	_isPingSent = false;
}