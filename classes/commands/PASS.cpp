/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:41 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 17:50:28 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PASS(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message(":" + get_server_addr() + " 461 * PASS :Not enough parameters");
		return (0);
	}
	if (_has_password)
	{
		send_message(":" + get_server_addr() + " 462 * PASS :You already registered");
		return (0);
	}
	if (command.parameters[0] != g_server.get_password())
	{
		send_message(":" + get_server_addr() + " 464 * PASS :Password incorrect");
		return (0);
	}
	_has_password = true;
	return (0);
}