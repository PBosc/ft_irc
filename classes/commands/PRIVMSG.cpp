/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:46 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 16:07:35 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PRIVMSG(t_command &command)
{

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message(":" + get_server_addr() + " 451 * PRIVMSG :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 411 * PRIVMSG :No recipient given");
		return (0);
	}
	std::cout << command.command << ": " << command.parameters[0] << ": " << command.suffix << std::endl;
	if (command.parameters[0].size() >= 2 && command.parameters[0][0] == '#')
	{
		Channel *channel;
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
		for (std::vector<std::string>::iterator it = g_server.get_ban_words().begin(); it != g_server.get_ban_words().end(); it++)
		{
			if (command.suffix.find(*it) != std::string::npos)
			{
				send_message(":bot!bot@" + get_server_addr() + " PRIVMSG " + _nick + " : You are not allowed to say that word");
				return (0);
			}
		}
		for (std::map<std::string, std::string>::iterator it = g_server.get_bot_responses().begin(); it != g_server.get_bot_responses().end(); it++)
		{
			if (command.suffix.find((*it).first, 0) == 0)
			{
				std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " PRIVMSG " + command.parameters[0] + " :" + command.suffix);
				channel->broadcast(message, _fd);
				message = ":bot!bot@" + get_server_addr() + " PRIVMSG " + command.parameters[0] + " :" + (*it).second;
				channel->broadcast(message, -42);
				return (0);
			}
		}
		std::string message = ":" + _nick + "!" + _user + "@" + get_addr() + " PRIVMSG " + command.parameters[0] + " :" + command.suffix;
		channel->broadcast(message, _fd);
		return (0);
	}
	if (command.parameters[0] == _nick)
	{
		send_message(":" + get_server_addr() + " 412 * PRIVMSG :Cannot send to self");
		return (0);
	}
	for (std::map<int, Client *>::iterator it = g_server.get_clients().begin(); it != g_server.get_clients().end(); it++)
	{
		if ((*it).second && (*it).second->get_nick() == command.parameters[0])
		{
			std::cout << "" << std::endl;
			std::stringstream ss(command.suffix);
			std::string first_word;
			ss >> first_word;
			(*it).second->send_message(":" + _nick + "!" + _user + "@" + get_addr() + " PRIVMSG " + command.parameters[0] + " :" + command.suffix + "\r\n");
			return (0);
		}
	}
	return (0);
}
