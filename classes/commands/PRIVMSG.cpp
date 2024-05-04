/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:46 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:40:37 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PRIVMSG(t_command &command)
{
	Channel	*channel;

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message("ERROR :You must set your password, nick and user before sending a message");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :Not enough parameters");
		return (0);
	}
	std::cout << command.command << ": " << command.parameters[0] << ": " << command.suffix << std::endl;
	if (command.parameters.at(0).size() >= 2 && command.parameters.at(0).at(0) == '#')
	{
		if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
			channel = g_server.get_channels()[command.parameters[0]];
		else
		{
			send_message("ERROR :No such channel " + command.parameters[0]);
			return (0);
		}
		if (!channel->is_user(_fd))
		{
			send_message("ERROR :You are not in channel "
				+ command.parameters[0]);
			return (0);
		}
		std::string message = ":" + _nick
			+ " PRIVMSG " + command.parameters[0] + " :" + command.suffix;
		channel->broadcast(message, _fd);
		return (0);
	}
	for (std::map<int,
		Client *>::iterator it = g_server.get_clients().begin(); it != g_server.get_clients().end(); it++)
	{
		if ((*it).second->get_nick() == command.parameters[0])
		{
			std::cout << "" << std::endl;
			(*it).second->send_message(":" + _nick
				+ " PRIVMSG " + command.parameters[0] + " :" + command.suffix);
			return (0);
		}
	}
	return (0);
}