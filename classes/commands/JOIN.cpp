/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:17 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:31:23 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_JOIN(t_command &command)
{
	Channel	*channel;

	if (!this->can_execute())
	{
		send_message("ERROR :You must set your password, nick and user before joining a channel");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No channel given");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) == g_server.get_channels().end())
		g_server.get_channels()[command.parameters[0]] = new Channel(command.parameters[0],
				_fd);
	channel = g_server.get_channels()[command.parameters[0]];
	if (channel->is_user(_fd))
	{
		send_message("ERROR :You are already in channel "
			+ command.parameters[0]);
		return (0);
	}
	if (channel->get_user_limit() != 0
		&& channel->get_users().size() >= channel->get_user_limit())
	{
		send_message("ERROR :Channel " + command.parameters[0] + " is full");
		return (0);
	}
	channel->add_user(_fd);
	return (0);
}