/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:25 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/05 22:49:47 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "IRC.hpp"

void Client::handle_i_mode(t_command &command, bool sign, Channel *channel)
{
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
	if (channel->get_invite_only() && sign)
	{
		send_message(":" + get_server_addr() + " 324 " + _nick + " " + command.parameters[0] + " +Cint");
		send_message(":" + get_server_addr() + " 324 " + _nick + " " + command.parameters[0] + " 0");
		return ;
	}
	channel->set_invite_only(sign);
	std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + (sign ? " +i" : " -i"));
	channel->broadcast(message, -42);
}

void Client::handle_t_mode(t_command &command, bool sign, Channel *channel)
{
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
	std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + (sign ? " +" : " -") + "t");
	channel->broadcast(message, -42);
}

void Client::handle_k_mode(t_command &command, bool sign, size_t &params, Channel *channel)
{
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
	std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + (sign ? " +k :" : " -k :") + (sign ? command.parameters.at(params) : "key removed"));
	channel->broadcast(message, -42);
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void Client::handle_l_mode(t_command &command, bool sign, size_t &params, Channel *channel)
{
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
		if (!is_number(command.parameters.at(params)))
		{
			send_message(":" + get_server_addr() + " 461 * MODE :Invalid limit");
			return ;
		}
		channel->set_limit(std::atoi(command.parameters.at(params).c_str()));
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

void Client::handle_o_mode(t_command &command, bool sign, size_t &params, Channel *channel)
{
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
	if (!can_execute()) {
		send_message(":" + get_server_addr() + " 451 * KILL :You have not registered");
		return 1;
	}
	if (!command.parameters.size())
	{
		send_message(":" + get_server_addr() + " 461 * MODE :Not enough parameters");
		return (0);
	}
	else
	{
		if (command.parameters.size() == 1)
		{
			if (command.parameters[0][0] == '#')
			{
				Channel *channel;
				if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
					channel = g_server.get_channels()[command.parameters[0]];
				else
				{
					send_message(":" + get_server_addr() + " 403 * " + command.parameters[0] + " :No such channel");
					return (0);
				}
				send_message(":" + get_server_addr() + " 324 * " + command.parameters[0] + " +" + (channel->get_invite_only() ? "i" : "") + (channel->get_topic_op_only() ? "t" : "") + (channel->get_key().length() ? "k": "") + (channel->get_limit_set() ? "l" : ""));
			}
		}
		if (command.parameters.size() >= 2 && command.parameters.at(0) == _nick)
		{
			if (command.parameters.at(1) == "+i")
			{
				_is_invisible = true;
				send_message(":" + get_server_addr() + " 221 " + _nick + " +i");
				return (0);
			}
			else
			{
				_is_invisible = false;
				std::string msg(":" + _nick + "!" + _user + "@3" + get_server_addr() + " MODE " + _nick + " -i");
				g_server.broadcast(msg, -42);
			}
			send_message(":" + get_server_addr() + " 461 * MODE :Invalid mode");
			return (0);
		}
		Channel *channel;
		if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
			channel = g_server.get_channels()[command.parameters[0]];
		else
		{
			send_message(":" + get_server_addr() + " 403 * " + command.parameters[0] + " :No such channel");
			return (0);
		}
		if (command.parameters.size() < 2)
		{
			std::string message(":" + _nick + "!" + _user + "@" + get_addr() + " MODE " + channel->get_name() + " :");
			return (0);
		}
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
					handle_i_mode(command, sign, channel);
					break;
				case 't':
					handle_t_mode(command, sign, channel);
					break;
				case 'k':
					handle_k_mode(command, sign, params, channel);
					++params;
					break;
				case 'l':
					handle_l_mode(command, sign, params, channel);
					++params;
					break;
				case 'o':
					handle_o_mode(command, sign, params, channel);
					++params;
					break;
				default:
					send_message(":" + get_server_addr() + " 461 * MODE :Invalid mode");
					return (0);
			}
		}
	}
	return (0);
}
