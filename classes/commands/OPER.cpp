/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:35 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:57:05 by pibosc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

bool is_nick_valid(std::string nick) {
	for (std::map<int, Client *>::iterator it = g_server.get_clients().begin(); it != g_server.get_clients().end(); it++) {
		if ((*it).second->get_nick() == nick)
			return true;
	}
	return false;
}


int Client::command_OPER(t_command &cmd)
{
	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message(":ft_irc 451 * OPER :You have not registered");
		return (0);
	}
	if (!is_nick_valid(cmd.parameters[0])) {
		send_message(":ft_irc 501 * :No such user by that nickname");
		return 0;
	}
	if (cmd.parameters[1] != "password")
	{
		send_message(":ft_irc 464 * :Password incorrect");
		return 0;
	}
	if (false) {
		send_message(":ft_irc NOTICE " + cmd.parameters[0] + " :You are already an IRC operator");
		return 0;
	}
	send_message("381 " + cmd.parameters[0] + " :You are now an IRC operator");
	return 1;
}