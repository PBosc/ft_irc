/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:25 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 04:25:56 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

void Client::handle_i_mode(t_command &command, bool sign)
{
	if (command.parameters.at(0) == _nick)
	{
		send_message(":" + get_server_addr() + " 381 * :You're now invisible");
		return ;
	}
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":" + get_server_addr() + " 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	if (!_is_operator && !channel->get_oper(_fd))
	{
		send_message(":" + get_server_addr() + " 482 * " + command.parameters[0] + " :You're not operator");
		return ;
	}
	channel->set_invite_only(sign);
	std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + " +i" + " :is " + (channel->get_invite_only() ? "now" : "not anymore") + " invite only");
	channel->broadcast(message, -42);
}

void Client::handle_t_mode(t_command &command, bool sign)
{
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":" + get_server_addr() + " 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	if (!_is_operator && !channel->get_oper(_fd))
	{
		send_message(":" + get_server_addr() + " 482 * " + command.parameters[0] + " :You're not operator");
		return ;
	}
	channel->set_topic_op_only(sign);
	std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + " " + command.parameters[0] + " :is " + (channel->get_topic_op_only() ? "now" : "not anymore") + " topic op only");
	channel->broadcast(message, -42);
}

void Client::handle_k_mode(t_command &command, bool sign, size_t &params)
{
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":" + get_server_addr() + " 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	if (!_is_operator && !channel->get_oper(_fd))
	{
		send_message(":" + get_server_addr() + " 482 * " + command.parameters[0] + " :You're not operator");
		return ;
	}
	if (command.parameters.size() <= params && sign)
	{
		send_message(":" + get_server_addr() + " 461 * MODE :Not enough parameters");
		return ;
	}
	channel->set_key(command.parameters.at(params));
	++params;
	std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + (sign ? " +" : " -") + "k" + " :" + (sign ? command.parameters[2] : "key removed"));
	channel->broadcast(message, -42);
}

void Client::handle_l_mode(t_command &command, bool sign, size_t &params)
{
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":" + get_server_addr() + " 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	if (!_is_operator && !channel->get_oper(_fd))
	{
		send_message(":" + get_server_addr() + " 482 * " + command.parameters[0] + " :You're not operator");
		return ;
	}
	if (command.parameters.size() < params + 1 && sign)
	{
		send_message(":" + get_server_addr() + " 461 * MODE :Not enough parameters");
		return ;
	}
	if (sign)
	{
		channel->set_limit(std::atoi(command.parameters.at(params).c_str()));
		++params;
		std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + " +l" + " :" + command.parameters.at(params));
		channel->broadcast(message, -42);
	}
	else
	{
		channel->unset_limit();
		std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + " -l" + " :limit removed");
		channel->broadcast(message, -42);
	}
}

void Client::handle_o_mode(t_command &command, bool sign, size_t &params)
{
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":" + get_server_addr() + " 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	if (!_is_operator && !channel->get_oper(_fd))
	{
		send_message(":" + get_server_addr() + " 482 * " + command.parameters[0] + " :You're not operator");
		return ;
	}
	if (command.parameters.size() <= params)
	{
		send_message(":" + get_server_addr() + " 461 * MODE :Not enough parameters");
		return ;
	}
	int	fd;
	for (size_t index = 0; index < channel->get_users().size(); ++index)
	{
		fd = channel->get_users().at(index);
		if (g_server.get_clients()[fd]->get_nick() == command.parameters.at(params))
		{
			channel->set_oper(fd, sign);
			std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + (sign ? " +" : " -") + "o" + " :" + g_server.get_clients()[fd]->get_nick() + (sign ? " is now an operator" : " is not an operator anymore"));
			channel->broadcast(message, -42);
			return ;
		}
	}
	send_message(":" + get_server_addr() + " 442 * " + command.parameters[0] + " MODE :User specified not in channel");
}

int Client::command_MODE(t_command &command)
{
	size_t	params;
	bool	sign;
	if (command.parameters.size() < 2)
	{
		send_message(":" + get_server_addr() + " 461 * MODE :Not enough parameters");
		return (0);
	}
	else
	{
		if (command.parameters.at(1).at(0) != '+' && command.parameters.at(1).at(0) != '-')
		{
			send_message(":" + get_server_addr() + " 461 * MODE :Invalid mode");
			return (0);
		}
		sign = command.parameters.at(1).at(0) == '+';
		params = 2;
		for (std::string::iterator it = command.parameters[1].begin() + 1; it != command.parameters[1].end(); it++)
		{
			switch (*it) 
			{
				case 'i':
					handle_i_mode(command, sign);
					break;
				case 't':
					handle_t_mode(command, sign);
					break;
				case 'k':
					handle_k_mode(command, sign, params);
					break;
				case 'l':
					handle_l_mode(command, sign, params);
					break;
				case 'o':
					handle_o_mode(command, sign, params);
					break;
				default:
					send_message(":" + get_server_addr() + " 461 * MODE :Invalid mode");
					return (0);
			}
		}
	}
	return (0);
}
