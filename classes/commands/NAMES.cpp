/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:28 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 16:55:25 by wouhliss         ###   ########.fr       */
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
		send_message(":" + get_server_addr() + " 451 * NAMES :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * NAMES :Not enough parameters");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
		channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message(":" + get_server_addr() + " 403 * " + command.parameters[0] + " :No such channel");
		return (0);
	}
	std::string names = "NAMES " + command.parameters[0] + " :";
	bool first = true;
	for (std::size_t i = 0; i < channel->get_users().size(); ++i)
	{
		int client_fd = channel->get_users()[i];
		Client *client = g_server.get_clients()[client_fd];
		if (!client)
			continue;
		if (!first)
			names.append(" ");
		if (channel->get_oper(client_fd))
			names.append("@");
		names.append(client->get_nick());
		first = false;
	}
	send_message(":" + get_server_addr() + " 353 " + _nick + " = " + command.parameters[0] + " :" + names);
	send_message(":" + get_server_addr() + " 366 " + _nick + " " + command.parameters[0] + " :End of NAMES list.");
	return (0);
}
