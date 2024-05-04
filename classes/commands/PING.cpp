/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:43 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:31:12 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PING(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No PING message given");
		return (0);
	}
	send_message("PONG " + command.parameters[0]);
	return (0);
}