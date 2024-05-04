/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:48 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 08:20:34 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_QUIT(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message(":ft_irc 461 * QUIT :Not enough parameters");
		return (0);
	}
	send_message(":ft_irc 221 * QUIT :Goodbye");
	g_server.kick_user(_fd);
	return (0);
}