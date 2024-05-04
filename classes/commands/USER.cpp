/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:58 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 05:00:57 by pibosc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_USER(t_command &command)
{
	if (_has_Client)
	{
		send_message(":ft_irc 462 * USER :You are already reregistered");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message(":ft_irc 461 * USER :Not enough parameters");
		return (0);
	}
	_user = command.parameters[0];
	_has_Client = true;
	return (0);
}