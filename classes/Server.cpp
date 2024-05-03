/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:49:02 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/02 23:38:29 by pibosc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

void	Server::run(int &i)
{
	if (g_server.get_epoll().events[i].data.fd == g_server.get_socket().fd)
	{
		user_connection();
		return ;
	}
	if ((g_server.get_epoll().events[i].events & EPOLLERR)
		|| (g_server.get_epoll().events[i].events & EPOLLHUP)
		|| (g_server.get_epoll().events[i].events & EPOLLRDHUP)
		|| !(g_server.get_epoll().events[i].events & EPOLLIN))
	{
		user_disconnection(i);
		return ;
	}
	handle_message(g_server.get_epoll().events[i].data.fd);
}

bool Server::init(int port, std::string password)
{
	int	sock_opt_val;

	_port = port;
	_password = password;
	sock_opt_val = 1;
	_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket.fd < 0)
	{
		std::cerr << "Error: socket() failed" << std::endl;
		return (false);
	}
	_clients = std::map<int, Client *>();
	_clients_id = 0;
	_socket.addr = sockaddr_in();
	_socket.addr.sin_family = AF_INET;
	_socket.addr.sin_port = htons(port);
	_socket.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (setsockopt(_socket.fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt_val,
			sizeof(sock_opt_val)) < 0)
	{
		std::cerr << "Error: setsockopt() failed" << std::endl;
		return (close(_socket.fd), false);
	}
	if (bind(_socket.fd, (struct sockaddr *)(&_socket.addr), sizeof(_socket.addr)) < 0)
	{
		std::cerr << "Error: bind() failed" << std::endl;
		return (close(_socket.fd), false);
	}
	if (listen(_socket.fd, MAX_CONNECTIONS) < 0)
	{
		std::cerr << "Error: listen() failed" << std::endl;
		return (close(_socket.fd), false);
	}
	_epoll.fd = epoll_create1(EPOLL_CLOEXEC);
	if (_epoll.fd < 0)
	{
		std::cerr << "Error: epoll_create1() failed" << std::endl;
		return (close(_socket.fd), false);
	}
	_socket.event.data.fd = _socket.fd;
	_socket.event.events = EPOLLIN;
	if (epoll_ctl(_epoll.fd, EPOLL_CTL_ADD, _socket.fd, &_socket.event) < 0)
	{
		std::cerr << "Error: epoll_ctl() failed" << std::endl;
		return (close(_socket.fd), close(_epoll.fd), false);
	}
	return (true);
}

Server::Server(void)
{
	_socket.fd = -1;
	_epoll.fd = -1;
}

Server::~Server(void)
{
	std::map<int, Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		close (it->second->get_fd());
		delete it->second;
	}
}

t_epoll &Server::get_epoll(void)
{
	return (_epoll);
}

void Server::set_epoll(t_epoll &epoll)
{
	_epoll = epoll;
}

t_socket &Server::get_socket(void)
{
	return (_socket);
}

void Server::set_socket(t_socket &socket)
{
	_socket = socket;
}

int &Server::get_port(void)
{
	return (_port);
}

void Server::set_port(int &port)
{
	_port = port;
}

std::string &Server::get_password(void)
{
	return (_password);
}

void Server::set_password(std::string &string)
{
	_password = string;
}

std::map<std::string, Channel *> &Server::get_channels(void)
{
	return (_channels);
}

void Server::set_channel(std::string &channel_name, Channel *channel)
{
	_channels[channel_name] = channel;
}

std::map<int, Client *> &Server::get_clients(void)
{
	return (_clients);
}

void Server::set_client(int &fd, Client *client)
{
	_clients[fd] = client;
}

int &Server::get_clients_id(void)
{
	return (_clients_id);
}

void Server::set_clients_id(int &clients_id)
{
	_clients_id = clients_id;
}

std::ostream &operator<<(std::ostream &os, Server &server)
{
	os << "Server port: " << server.get_port() << std::endl;
	os << "Server password: " << server.get_password() << std::endl;
	return os;
}
