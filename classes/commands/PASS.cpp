/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:41 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 05:07:45 by pibosc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_PASS(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message(":ft_irc 461 * PASS :Not enough parameters");
		return (0);
	}
	if (_has_password)
	{
		send_message(":ft_irc 462 * PASS :You already registered");
		return (0);
	}
	if (command.parameters[0] != g_server.get_password())
	{
		send_message(":ft_irc 464 * PASS :Password incorrect");
		return (0);
	}
	_has_password = true;
	return (0);
}