/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:43 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 22:50:04 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PING(t_command &command)
{
	if (!can_execute())
	{
		send_message(":" + get_server_addr() + " 451 " + _nick + " PING :You have not registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 " + _nick + " PING :Not enough parameters");
		return (0);
	}
	send_message(":" + get_server_addr() + " PONG " + get_server_addr() + " " + command.parameters[0]);
	return (0);
}
