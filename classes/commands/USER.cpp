/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:58 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 18:55:00 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_USER(t_command &command)
{
	if (_has_Client)
	{
		send_message(":" + get_server_addr() + " 462 * USER :You are already registered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * USER :Not enough parameters");
		return (0);
	}
	if (command.suffix == "") {
		send_message(":" + get_server_addr() + " 461 * USER :Not enough parameters");
		return (0);
	}
	_user = command.parameters[0];
	_real_name = command.suffix;
	_has_Client = true;
	return (0);
}
