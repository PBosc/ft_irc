/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:54:39 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/04 01:11:18 by wouhliss         ###   ########.fr       */
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
	closed = false;
	while (true)
	{
		rc = recv(fd, buf, BUFFER_SIZE, 0);
		if (rc <= 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Error: recv() failed: " << strerror(errno) << std::endl;
				closed = true;
			}
			std::stringstream ss(client->get_message());
			while (std::getline(ss, line))
			{
				std::cout << line << std::endl;
				t_command cmd;
				if (line.size() == client->get_message().size())
					break ;
				std::stringstream	stream(line);
				stream >> cmd.command;
				std::string param;
				while (stream >> param)
				{
					if (param.size() && param.at(0) == ':')
					{
						param.erase(0, 1);
						cmd.suffix = param;
						while (stream >> param)
							cmd.suffix.append(" " + param);
						break ;
					}
					cmd.parameters.push_back(param);
				}
				if (g_server.get_commands().find(cmd.command) != g_server.get_commands().end())
				{
					if ((client->*g_server.get_commands()[cmd.command])(cmd))
						return ;
				}
				else
					(client->*g_server.get_commands()["UNKNOWN"])(cmd);
				client->get_message().erase(0, line.size() + 1);
			}
			break ;
		}
		buf[rc] = 0;
		client->get_message().append(buf);
	}
}
