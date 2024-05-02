/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:59:34 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/02 16:47:17 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"

void	run_serv(t_data &data, int i)
{
	if (data.epoll.events[i].data.fd == data.socket.fd)
	{
		user_connection(data);
		return ;
	}
	if ((data.epoll.events[i].events & EPOLLERR)
		|| (data.epoll.events[i].events & EPOLLHUP)
		|| (data.epoll.events[i].events & EPOLLRDHUP)
		|| !(data.epoll.events[i].events & EPOLLIN))
	{
		user_disconnection(data, i);
		return ;
	}
	// handle_message(data, data.epoll.events[i].data.fd);
} 
