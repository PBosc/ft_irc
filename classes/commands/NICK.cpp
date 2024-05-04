/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:30 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 18:37:33 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

bool	erroneous_nick(std::string nick)
{
	std::string specials = "[]|^_-{}";

	if (nick.length() > 9)
		return false;
	for (std::string::iterator it = nick.begin(); it != nick.end(); it++)
	{
		if (!isalnum(*it) && specials.find(*it) == std::string::npos)
			return (true);
	}
	return (false);
}

int Client::command_NICK(t_command &cmd)
{
	std::string old_nick = _nick;

	if (cmd.parameters.size() < 1) { // useless for the most part
		send_message(":" + get_server_addr() + " 421 * " + _nick
			+ " NICK :Command requires one argument");
		return (0);
	}
	if (cmd.parameters.size() > 1) {
		send_message(":" + get_server_addr() + " 421 * " + _nick
			+ " NICK :Command takes only one argument");
		return (0);
	}
	if (erroneous_nick(cmd.parameters[0])) {
		send_message(":" + get_server_addr() + " 432 * " + cmd.parameters[0]
			+ " :Erroneous nickname");
		return (0);
	}
	if (is_already_in_use(cmd.parameters[0])) {
		send_message(":" + get_server_addr() + " 433 * " + cmd.parameters[0]
	+ " :Nickname is already in use");
		return (0);
	}
	_nick = cmd.parameters[0];
	send_message(":" + old_nick + "!" + _user + "@" + get_server_addr() + " NICK " + _nick);
	_has_nick = true;
	return (0);
}
