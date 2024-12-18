/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IO_Loop.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:42:24 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/17 15:47:20 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>
#include <array>
#include <cstring>
#include <algorithm>
#include <csignal>

static void	debug_print_revents(short revents)
{
	if (revents & POLLOUT)
		std::cout << "POLLOUT ";
	if (revents & POLLIN)
		std::cout << "POLLIN ";
	if (revents & POLLHUP)
		std::cout << "POLLHUP ";
	if (revents & POLLERR)
		std::cout << "POLLERR ";
	std::cout << std::endl;
}

void Server::_acceptClient()
{
	int			clientFd;
	sockaddr_in	clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);
	const auto	clientPollFD =
		std::find_if(_pollFDs.begin(), _pollFDs.end(),
			[](const pollfd &pollFD) { return pollFD.fd == -1; });

	clientFd = accept(_socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientFd == -1 && errno != EINTR)
		throw std::runtime_error("Error accepting a client connection");
	clientPollFD->fd = clientFd;
	_clients[clientFd].setHostname(clientAddr);
	clientPollFD->revents = 0;
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error setting the client socket to non-blocking");
	std::cout << "Client " << clientFd << " connected" << std::endl;
}

void Server::_disconnectClient(pollfd &pollFD)
{
	if (pollFD.fd == -1)
		return ;
	debug_print_revents(pollFD.revents);
	std::cout << "Client " << pollFD.fd << " disconnected" << std::endl;
	Channel::clientDisconnected(_clients[pollFD.fd]);
	close(pollFD.fd);
	_clients.erase(pollFD.fd);
	pollFD.fd = -1;
	pollFD.revents = 0;
}

void	Server::_initPollFDs()
{
	for (auto &pollFD : _pollFDs)
	{
		pollFD.fd = -1;
		pollFD.events = POLLIN | POLLHUP | POLLERR | POLLOUT;
		pollFD.revents = 0;
	}
	_pollFDs[0].events = POLLIN;
}

void	Server::sigAction(int sig)
{
	_sig = true;
	if (sig == SIGINT)
	{
		_instance->reload();
		_sig = false;
	}
}

void Server::_startLoop()
{
	while (!_sig)
	{
		if (poll(_pollFDs.data(), _clients.size() + 1, 0) == -1 && errno != EINTR)
			throw std::runtime_error(string("Poll Error: ") + strerror(errno));
		if (_pollFDs[0].revents & POLLIN)
			_acceptClient();
		for (auto &clientPollFD : _pollFDs)
		{
			if (clientPollFD.fd == -1 || clientPollFD.fd == _socketFd)
				continue ;
			if (clientPollFD.revents & POLLIN)
				_receiveMessage(clientPollFD);
			if (clientPollFD.revents & POLLHUP || clientPollFD.revents & POLLERR)
				_disconnectClient(clientPollFD);
			if (clientPollFD.revents & POLLOUT)
				_sendClientBuffer(clientPollFD.fd);
			clientPollFD.revents = 0;
			_checkConnectionTimeout(clientPollFD);
			Channel::cleanUnusedChannels();
		}
		_pollFDs[0].revents = 0;
	}
}
