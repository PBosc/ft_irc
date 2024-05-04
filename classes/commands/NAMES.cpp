/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:28 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:31:35 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_NAMES(t_command &command)
{
	Channel	*channel;

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message("ERROR :You must set your password, nick and user before listing names");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No channel given");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
		channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message("ERROR :No such channel " + command.parameters[0]);
		return (0);
	}
	std::string names = "NAMES " + command.parameters[0] + " :";
	for (std::size_t i = 0; i < channel->get_users().size(); ++i)
	{
		// int client_fd = channel->get_users().at(i);
		// Client *client = g_data->clients[client_fd];
	}
	send_message(names);
	return (0);
}