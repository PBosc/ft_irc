/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UNKNOWN.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:53 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 05:01:06 by pibosc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_unknown(t_command &command)
{
	send_message(":ft_irc 421 * " + command.command + " :Unknown command");
	return 0;
}