/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:01:15 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/02 19:15:10 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"

void	user_connection(t_data &data)
{
	int					fd_new_con;
	struct sockaddr_in	socket_new_con;
	struct epoll_event	epoll_event_new_con;
	socklen_t			size_socket_new_con;

	size_socket_new_con = sizeof(socket_new_con);
	socket_new_con = sockaddr_in();
	epoll_event_new_con = epoll_event();
	fd_new_con = accept(data.socket.fd, (struct sockaddr *)&socket_new_con,
			&size_socket_new_con);
	if (fd_new_con < 0)
	{
		std::cerr << "Error: accept() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	epoll_event_new_con.events = EPOLLIN | EPOLLRDHUP;
	epoll_event_new_con.data.fd = fd_new_con;
	fcntl(fd_new_con, F_SETFL, O_NONBLOCK);
	if (epoll_ctl(data.epoll.fd, EPOLL_CTL_ADD, fd_new_con,
			&epoll_event_new_con) < 0)
	{
		std::cerr << "Error: epoll_ctl() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "User connected with fd: " << fd_new_con << std::endl;
	Client *client = new Client(fd_new_con, ++data.client_id);
	data.clients[fd_new_con] = client;
}

void	user_disconnection(t_data &data, int i)
{
	epoll_ctl(data.epoll.fd, EPOLL_CTL_DEL, data.epoll.events[i].data.fd, &data.socket.event);
	close(data.epoll.events[i].data.fd);
	std::cout << "User with fd: " << data.epoll.events[i].data.fd << " disconnected" << std::endl;
	delete data.clients[data.epoll.events[i].data.fd];
	data.clients.erase(data.epoll.events[i].data.fd);
}
