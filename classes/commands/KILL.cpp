/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KILL.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:22 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:32:50 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_KILL(t_command &command)
{
	if (!_has_password || !_has_Client || !_has_nick)
	{
		send_message("ERROR :You must set your password, nick and user before killing a user");
		return (0);
	}
	if (command.parameters.size() < 2)
	{
		send_message("ERROR :Not enough parameters");
		return (0);
	}
	if (!_is_operator)
	{
		send_message("ERROR :You are not an operator");
		return (0);
	}
	return (0);
}