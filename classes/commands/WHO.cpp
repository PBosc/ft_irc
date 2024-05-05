/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 17:59:09 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 18:51:26 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_WHO(t_command &command)
{
	Channel	*channel;

	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message(":" + get_server_addr() + " 451 * WHO :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * WHO :Not enough parameters");
		return (0);
	}
	if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
		channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message(":" + get_server_addr() + " 403 * " + command.parameters[0] + " :No such channel");
		return (0);
	}
	
	for (std::size_t i = 0; i < channel->get_users().size(); ++i)
	{
		int client_fd = channel->get_users()[i];
		Client *client = g_server.get_clients()[client_fd];
		if (!client)
			continue;
		std::string spe = client->is_operator() ? "*" : (channel->get_oper(client_fd) ? "@" : "");
		send_message(":" + get_server_addr() + " 352 " + _nick + " " + channel->get_name() + " ~u " + client->get_addr() + " " + get_server_addr() + " " + client->get_nick() + " H" + spe + " :0 " + client->get_real_name());
	}
	send_message(":" + get_server_addr() + " 315 " + _nick + " " + command.parameters[0] + " :End of WHO list.");
	return 1;
}