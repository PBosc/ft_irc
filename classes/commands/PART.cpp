/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:38 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:32:22 by ybelatar         ###   ########.fr       */
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
		send_message("ERROR :You must set your password, nick and user before leaving a channel");
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
	if (!channel->is_user(_fd))
	{
		send_message("ERROR :You are not in channel " + command.parameters[0]);
		return (0);
	}
	channel->part_user(_fd);
	return (0);
}