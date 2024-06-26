/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:13 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/06 00:37:41 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_INVITE(t_command &cmd)
{
	if (!_has_password || !_has_Client || !_has_nick) {
		send_message(":" + get_server_addr() + " 451 * INVITE :You have not registered");
		return 1;
	}
	if (cmd.parameters.size() < 2)
	{
		send_message(":" + get_server_addr() + " 461 * INVITE :Not enough parameters");
		return (0);
	}
	Client *user = g_server.find_client_by_nick(cmd.parameters[0]);
	if (!user) {
		send_message(":" + get_server_addr() + " 501 * :No user by that nickname");
		return 0;
	}
	std::map<std::string, Channel *>::iterator it = g_server.get_channels().find(cmd.parameters[1]);
	if (it == g_server.get_channels().end()) {
		send_message(":" + get_server_addr() + " 501 * :No channel by that name");
		return 0;
	}
	Channel *channel = (*it).second;
	if (channel->is_user(user->get_fd())) {
		send_message(":" + get_server_addr() + " 501 * :User already in channel");
		return 0;
	}
	if (!channel->get_invite_only())
	{
		send_message(":" + get_server_addr() + " 501 * :Channel is not invite only");
		return 0;
	}
	send_message(":" + get_server_addr() + " 341 " + _nick + " " + user->get_nick() + " " + channel->get_name());
	channel->invite(user->get_fd(), user);
	user->send_message(":" + _nick + "!" + _user + "@" + get_addr() + " INVITE " + user->get_nick() + " " + channel->get_name());
	return 1;
}
