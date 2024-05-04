/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:50 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 21:56:14 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_TOPIC(t_command &command)
{
	Channel	*channel;

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message(":" + get_server_addr() + " 451 * TOPIC :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * TOPIC :Not enough parameters");
		return (0);
	}
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
	if (channel->get_topic_op_only() && !_is_operator && !channel->get_oper(_fd))
	{
		send_message(":" + get_server_addr() + " 482 * " + command.parameters[0] + " :You're not operator");
		return (0);
	}
	if (command.has_suffix)
	{
		std::cout << command.suffix << std::endl;
		std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " TOPIC " + command.parameters[0] + " :" + command.suffix);
		channel->set_topic(command.suffix);
		channel->broadcast(message, -42);
	}
	else
		send_message(":" + get_server_addr() + " 332 * " + command.parameters[0] + " :" + channel->get_topic());
	return (0);
}
