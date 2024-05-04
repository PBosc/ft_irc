/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:58 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:30:54 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_USER(t_command &command)
{
	if (_has_Client)
	{
		send_message("ERROR :You have already set your Client");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No Client given");
		return (0);
	}
	_user = command.parameters[0];
	_has_Client = true;
	return (0);
}