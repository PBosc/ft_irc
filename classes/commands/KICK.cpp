/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:19 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 04:11:48 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"


int Client::command_KICK(t_command &command)
{
	Channel	*channel;
	Client *target;

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message(":" + get_server_addr() + " 451 * KICK :You have not registered");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
		channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message(":" + get_server_addr() + " 501 * :No channel named " + command.parameters[0]);
		return (0);
	}
	if (!_is_operator && !channel->get_oper(_fd))
	{
		send_message(":" + get_server_addr() + " 501 * :You are not a channel operator");
		return (0);
	}
	if (!channel->is_user(_fd))
	{
		send_message(":" + get_server_addr() + " 501 * :You are not in channel " + command.parameters[0]);
		return (0);
	}
	target = g_server.find_client_by_nick(command.parameters[1]);
	if (target == NULL) {
		send_message(":" + get_server_addr() + " 501 * :No user called " + command.parameters[1]);
		return 0;
	}
	if (!channel->is_user(target->get_fd())) {
		send_message(":" + get_server_addr() + " 501 * :The user you want to kick is not in channel " + command.parameters[0]);
		return (0);
	}
	target->send_message(":" + target->get_server_addr() + " 381 " + command.parameters[1] + " :You have been kicked from channel " + command.parameters[0] + " : " + command.suffix);
	channel->kick_user(target->get_fd());
	return (0);
}