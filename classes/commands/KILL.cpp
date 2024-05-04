/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KILL.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:22 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 05:59:56 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_KILL(t_command &cmd)
{
	if (!_has_password || !_has_Client || !_has_nick) {
		send_message(":ft_irc 451 * KILL :You have not registered");
		return 1;
	}
	if (!_is_operator) {
		send_message(":ft_irc 501 * :You are not an operator");
		return 1;
	}
	if (!is_already_in_use(cmd.parameters[0])) {
		send_message(":ft_irc 501 * :There is no such user");
		return 0;
	}
	g_server.kick_user(g_server.find_client_by_nick(cmd.parameters[0])->get_fd());
	send_message(":ft_irc 381 " + cmd.parameters[0] + " :User " + cmd.parameters[0] + " has been kicked from server. Reason : " + cmd.suffix);
	return 1;
}