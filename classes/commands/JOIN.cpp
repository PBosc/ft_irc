/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:17 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 05:53:47 by ybelatar         ###   ########.fr       */
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
		send_message(":" + get_server_addr() + " 462 * JOIN :You are already in a channel");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * JOIN :Not enough parameters");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) == g_server.get_channels().end())
	{
		channel = new Channel(command.parameters[0]);
		g_server.set_channel(command.parameters[0], channel);
		channel->add_user(_fd);
		channel->set_oper(_fd, true);
		std::string message = ":" + _nick + "!" + _user + "@" + get_addr() + " JOIN " + command.parameters[0];
		send_message(message);
		std::string names = "@" + _nick;
		send_message(":" + get_server_addr() + " 353 " + _nick + " = " + command.parameters[0] + " : " + names);
		send_message(":" + get_server_addr() + " 366 " + _nick + " " + command.parameters[0] + " :End of NAMES list.");
		return (0);
	}
	channel = g_server.get_channels()[command.parameters[0]];
	std::map<int, Client *> invited = channel->get_invited();
	if (channel->get_invite_only() && (!invited.size() || invited.find(_fd) == invited.end()))
	{
		send_message(":" + get_server_addr() + " 473 * " + command.parameters[0] + " :Cannot join channel (+i)");
		return (0);
	}
	if (channel->get_key() != "" && command.parameters.size() < 2)
	{
		send_message(":" + get_server_addr() + " 475 * " + command.parameters[0] + " :Cannot join channel (+k)");
		return (0);
	}
	if (channel->get_key() != "" && command.parameters[1] != channel->get_key())
	{
		send_message(":" + get_server_addr() + " 475 * " + command.parameters[0] + " :Cannot join channel (+k)");
		return (0);
	}
	if (channel->is_user(_fd))
	{
		send_message(":" + get_server_addr() + " 462 * JOIN :You are already in that channel");
		return (0);
	}
	if (channel->get_user_limit() != 0
		&& channel->get_users().size() >= channel->get_user_limit())
	{
		send_message(":" + get_server_addr() + " 471 * " + command.parameters[0] + " :Channel full");
		return (0);
	}
	channel->add_user(_fd);
	std::string message = ":" + _nick + "!" + _user + "@" + get_addr() + " JOIN " + command.parameters[0];
	channel->broadcast(message, -42);
	std::string names = "";
	bool first = true;
	for (std::size_t i = 0; i < channel->get_users().size(); ++i)
	{
		int client_fd = channel->get_users()[i];
		Client *client = g_server.get_clients()[client_fd];
		if (!client)
			continue;
		if (!first)
			names.append(" ");
		names.append((channel->get_oper(client_fd) ? "@" : "") + client->get_nick());
		first = false;
	}
	send_message(":" + get_server_addr() + " 353 " + _nick + " = " + command.parameters[0] + " :" + names);
	send_message(":" + get_server_addr() + " 366 " + _nick + " " + command.parameters[0] + " :End of NAMES list.");
	return (0);
}
