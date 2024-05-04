/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UNKNOWN.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:53 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 21:28:53 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_unknown(t_command &command)
{
	send_message(":" + get_server_addr() + " 421 * " + command.command + " :Unknown command");
	return 0;
}
