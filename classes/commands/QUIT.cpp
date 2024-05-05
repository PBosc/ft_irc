/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:48 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 22:50:17 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_QUIT(t_command &command)
{
	if (!can_execute())
	{
		send_message(":" + get_server_addr() + " 451 * QUIT :You have not registered");
		return (0);
	}
	if (command.suffix.empty())
	{
		send_message(":" + get_server_addr() + " 221 * QUIT :Goodbye");
		g_server.kick_user(_fd);
		return (0);
	}
	send_message(":" + get_server_addr() + " 221 * QUIT :" + command.suffix);
	g_server.kick_user(_fd);
	return (0);
}
