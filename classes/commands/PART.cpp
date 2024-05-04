/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:38 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 05:08:17 by pibosc           ###   ########.fr       */
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
		send_message(":ft_irc 451 * PART :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":ft_irc 461 * PART :Not enough parameters");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
		channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message(":ft_irc 403 * " + command.parameters[0] + " :No such channel");
		return (0);
	}
	if (!channel->is_user(_fd))
	{
		send_message(":ft_irc 442 * " + command.parameters[0] + " :You're not on that channel");
		return (0);
	}
	channel->part_user(_fd);
	return (0);
}