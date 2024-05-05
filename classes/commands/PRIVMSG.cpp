/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:46 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 23:22:06 by wouhliss         ###   ########.fr       */
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
		std::string msg("");
		for (std::vector<std::string>::iterator it = command.parameters.begin() + 1; it != command.parameters.end(); it++)
		{
			if (it != command.parameters.begin() + 1)
				msg += " ";
			msg += *it;
		}
		std::string message = ":" + _nick + "!" + _user + "@" + get_addr() + " PRIVMSG " + command.parameters[0] + " :" + msg + (msg.size() && command.suffix.size() ? " " : "") + command.suffix;
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
			std::string msg("");
			for (std::vector<std::string>::iterator it = command.parameters.begin() + 1; it != command.parameters.end(); it++)
			{
				if (it != command.parameters.begin() + 1)
					msg += " ";
				msg += *it;
			}
			std::string message = ":" + _nick + "!" + _user + "@" + get_addr() + " PRIVMSG " + command.parameters[0] + " :" + msg + (msg.size() && command.suffix.size() ? " " : "") + command.suffix;
			(*it).second->send_message(":" + _nick + "!" + _user + "@" + get_addr() + " PRIVMSG " + command.parameters[0] + " :" + command.suffix + "\r\n");
			return (0);
		}
	}
	send_message(":" + get_server_addr() + " 401 * " + command.parameters[0] + " :No such nick/channel");
	return (0);
}
