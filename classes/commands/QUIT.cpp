/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:48 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:32:11 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_QUIT(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No quit message given");
		return (0);
	}
	send_message("ERROR :Closing connection: " + command.parameters[0]);
	return (0);
}