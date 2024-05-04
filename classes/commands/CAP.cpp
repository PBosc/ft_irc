/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:28:59 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 17:45:51 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_CAP(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No capability given");
		return (0);
	}
	if (command.parameters[0] == "LS")
		send_message("CAP * LS :cap1 cap2 cap3");
	else if (command.parameters[0] == "END")
	{
		if (!_has_password)
		{
			send_message("ERROR :You must set your password before ending capabilities");
			g_server.kick_user(_fd);
			return (1);
		}
		if (!_has_nick)
		{
			send_message("ERROR :You must set your nick before ending capabilities");
			g_server.kick_user(_fd);
			return (1);
		}
		if (!_has_Client)
		{
			send_message("ERROR :You must set your Client before ending capabilities");
			g_server.kick_user(_fd);
			return (1);
		}
		send_message(":" + get_server_addr() + " 001 " + _nick + " :Welcome to the Internet Relay Network " + _nick + "!" + _user + "@" + get_addr());
	}
	else
		send_message("ERROR :Invalid capability");
	return (0);
}
