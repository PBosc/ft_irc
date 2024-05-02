/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:54:39 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/02 23:10:44 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"

void	handle_message(int fd)
{
	char	buf[BUFFER_SIZE + 1];
	ssize_t	rc;
	bool	closed;
	Client	*client;
	std::string line;

	client = g_server.get_clients()[fd];
	std::cout << "User " << fd << " is sending a command" << std::endl;
	closed = false;
	while (true)
	{
		rc = recv(fd, buf, BUFFER_SIZE, 0);
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Error: recv() failed: " << strerror(errno) << std::endl;
				closed = true;
			}
			std::stringstream ss(client->get_message());
			while (std::getline(ss, line))
			{
				if (line.size() == client->get_message().size())
					break ;
				std::cout << "User " << fd << " sent: " << line << std::endl;
				client->get_message().erase(0, line.size() + 1);
			}
			break ;
		}
		buf[rc] = 0;
		client->get_message().append(buf);
	}
}
