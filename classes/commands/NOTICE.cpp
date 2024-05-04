/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:33 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 19:04:22 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_NOTICE(t_command &command)
{
	Channel	*channel;

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message(":" + get_server_addr() + " 451 * NOTICE :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * NOTICE :Not enough parameters");
		return (0);
	}
	std::cout << command.command << ": " << command.parameters[0] << ": " << command.suffix << std::endl;
	if (command.parameters[0].size() >= 2 && command.parameters[0][0] == '#')
	{
		if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
			channel = g_server.get_channels()[command.parameters[0]];
		else
		{
			send_message(":" + get_server_addr() + " 403 * " + command.parameters[0] + " :No such channel");
			return (0);
		}
		if (!channel->is_user(_fd))
		{
			send_message(":" + get_server_addr() + " 442 * " + command.parameters[0] + " :You're not on that channel");
			return (0);
		}
		std::string message = ":" + _nick + "!" + _user + "@" + get_addr() + " NOTICE " + command.parameters[0] + " :" + command.suffix;
		channel->broadcast(message, _fd);
		return (0);
	}
	for (std::map<int,
		Client *>::iterator it = g_server.get_clients().begin(); it != g_server.get_clients().end(); it++)
	{
		if ((*it).second && (*it).second->get_nick() == command.parameters[0])
		{
			std::cout << "" << std::endl;
			std::stringstream ss(command.suffix);
			std::string first_word;
			ss >> first_word;
			(*it).second->send_message(":" + _nick + "!" + _user + "@" + get_addr() + " NOTICE " + command.parameters[0] + " :" + command.suffix + "\r\n");
			return (0);
		}
	}
	return (0);
}