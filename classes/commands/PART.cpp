/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:38 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 18:45:04 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PART(t_command &command)
{
	Channel	*channel;

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message(":" + get_server_addr() + " 451 * PART :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * PART :Not enough parameters");
		return (0);
	}
	std::stringstream ss(command.parameters[0]);
	std::string chan;
	while (getline(ss, chan, ','))
	{
		if (g_server.get_channels().find(chan) != g_server.get_channels().end())
			channel = g_server.get_channels()[chan];
		else
		{
			send_message(":" + get_server_addr() + " 403 * " + chan + " :No such channel");
			return (0);
		}
		if (!channel->is_user(_fd))
		{
			send_message(":" + get_server_addr() + " 442 * " + chan + " :You're not on that channel");
			return (0);
		}
		channel->part_user(_fd, command.suffix);
	}
	return (0);
}
