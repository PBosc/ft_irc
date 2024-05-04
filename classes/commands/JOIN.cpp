/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:17 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 05:25:26 by pibosc           ###   ########.fr       */
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
		send_message(":ft_irc 462 * JOIN :You are already in a channel");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":ft_irc 461 * JOIN :Not enough parameters");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) == g_server.get_channels().end())
		g_server.get_channels()[command.parameters[0]] = new Channel(command.parameters[0],
				_fd);
	channel = g_server.get_channels()[command.parameters[0]];
	if (channel->is_user(_fd))
	{
		send_message(":ft_irc 462 * JOIN :You are already in that channel");
		return (0);
	}
	if (channel->get_user_limit() != 0
		&& channel->get_users().size() >= channel->get_user_limit())
	{
		send_message(":ft_irc 471 * " + command.parameters[0] + " :Channel full");
		return (0);
	}
	channel->add_user(_fd);
	send_message(":" + _nick + " JOIN " + command.parameters[0]);
	return (0);
}