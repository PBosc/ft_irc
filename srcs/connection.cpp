/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:01:15 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/05 04:05:58 by pibosc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"

void	user_connection(void)
{
	int					fd_new_con;
	struct sockaddr_in	socket_new_con;
	struct epoll_event	epoll_event_new_con;
	socklen_t			size_socket_new_con;

	size_socket_new_con = sizeof(socket_new_con);
	socket_new_con = sockaddr_in();
	epoll_event_new_con = epoll_event();
	fd_new_con = accept(g_server.get_socket().fd, (struct sockaddr *)&socket_new_con,
			&size_socket_new_con);
	if (fd_new_con < 0)
	{
		if (errno != EWOULDBLOCK)
			std::cerr << "accept() failed" << std::endl;
		return ;
	}
	epoll_event_new_con.events = EPOLLIN | EPOLLRDHUP;
	epoll_event_new_con.data.fd = fd_new_con;
	fcntl(fd_new_con, F_SETFL, O_NONBLOCK);
	if (epoll_ctl(g_server.get_epoll().fd, EPOLL_CTL_ADD, fd_new_con,
			&epoll_event_new_con) < 0)
	{
		std::cerr << "Error: epoll_ctl() failed" << std::endl;
		close(fd_new_con);
		return ;
	}
	std::cout << "User connected with fd: " << fd_new_con << std::endl;
	Client *client = new Client(fd_new_con, ++g_server.get_clients_id(), socket_new_con);
	g_server.get_clients()[fd_new_con] = client;
}

void	user_disconnection(int &i)
{
	if (epoll_ctl(g_server.get_epoll().fd, EPOLL_CTL_DEL, g_server.get_epoll().events[i].data.fd, &g_server.get_socket().event) < 0)
	{
		std::cerr << "Error: epoll_ctl() failed" << std::endl;
		g_server.kick_user(g_server.get_epoll().events[i].data.fd);
		return ;
	}
	std::cout << "User with fd: " << g_server.get_epoll().events[i].data.fd << " disconnected" << std::endl;
	g_server.kick_user(g_server.get_epoll().events[i].data.fd);
	if (g_server.get_epoll().events[i].data.fd == g_server.get_bot_fd())
		g_server.set_bot_fd(-1);
}
