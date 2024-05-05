/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:35 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 06:53:17 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

bool is_operator_valid(std::string nick) {
	for (std::map<std::string, std::string>::iterator it = g_server.get_operators().begin(); it != g_server.get_operators().end(); it++) {
		if ((*it).first == nick)
			return true;
	}
	return false;
}



int Client::command_OPER(t_command &cmd)
{
	if (!_has_password || !_has_Client || !_has_nick) {
		send_message(":" + get_server_addr() + " 451 * OPER :You have not registered");
		return 0;
	}
	if (_is_operator) {
		send_message(":" + get_server_addr() + " 501 * :You are already an IRC operator");
		return 0;
	}
	if (!is_operator_valid(cmd.parameters[0])) {
		send_message(":" + get_server_addr() + " 501 * :No such operator by that name");
		return 0;
	}
	if (cmd.parameters[1] != g_server.get_operators()[cmd.parameters[0]]) {
		send_message(":" + get_server_addr() + " 464 * :Password incorrect");
		return 0;
	}
	send_message(":" + get_server_addr() + " 381 " + cmd.parameters[0] + " :You are now an IRC operator");
	_is_operator = true;
	return 1;
}