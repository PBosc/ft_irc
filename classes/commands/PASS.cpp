/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:41 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:30:24 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PASS(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No password given");
		return (0);
	}
	if (_has_password)
	{
		send_message("ERROR :You have already set your password");
		return (0);
	}
	if (command.parameters[0] != g_server.get_password())
	{
		send_message("ERROR :Invalid password");
		return (0);
	}
	_has_password = true;
	return (0);
}