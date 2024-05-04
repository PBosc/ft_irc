/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UNKNOWN.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:53 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 04:33:53 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

int Client::command_unknown(t_command &command)
{
	send_message("ERROR :Unknown command " + command.command);
	return 0;
}