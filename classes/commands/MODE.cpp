/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 04:29:25 by ybelatar          #+#    #+#             */
/*   Updated: 2024/05/04 07:47:08 by pibosc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

void Client::handle_i_mode(t_command &command)
{
	if (command.parameters.size() > 2)
	{
		send_message(":ft_irc 461 * MODE :Too many parameters");
		return ;
	}
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":ft_irc 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	channel->set_invite_only(command.parameters[1][0] == '+');
	channel->broadcast(_nick + " MODE " + channel->get_name() + " +i" + " :is " + (channel->get_invite_only() ? "now" : "not anymore") + " invite only", -42);
}

void Client::handle_t_mode(t_command &command)
{
	if (command.parameters.size() > 2)
	{
		send_message(":ft_irc 461 * MODE :Too many parameters");
		return ;
	}
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":ft_irc 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	channel->set_topic_op_only(command.parameters[1][0] == '+');
	channel->broadcast(_nick + " MODE " + channel->get_name() + " +t" + + (channel->get_topic_op_only() ? "now" : "not anymore") + " :is now topic set", -42);
}

void Client::handle_k_mode(t_command &command)
{
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":ft_irc 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	if (command.parameters.size() < 3 && command.parameters[1][0] == '+')
	{
		send_message(":ft_irc 461 * MODE :Not enough parameters");
		return ;
	}
	if (command.parameters[1][0] == '+')
	{
		channel->set_key(command.parameters[2]);
		channel->broadcast(_nick + " MODE " + channel->get_name() + " +k" + " :" + command.parameters[2], -42);
	}
	else if (command.parameters[1][0] == '-')
	{
		channel->unset_key();
		channel->broadcast(_nick + " MODE " + channel->get_name() + " -k" + " :key removed", -42);
	}
}

void Client::handle_l_mode(t_command &command)
{
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":ft_irc 442 * " + command.parameters[0] + " :You're not on that channel");
		return ;
	}
	if (command.parameters.size() < 3 && command.parameters[1][0] == '+')
	{
		send_message(":ft_irc 461 * MODE :Not enough parameters");
		return ;
	}
	if (command.parameters[1][0] == '+')
	{
		channel->set_limit(std::atoi(command.parameters[2].c_str()));
		channel->broadcast(_nick + " MODE " + channel->get_name() + " +l" + " :" + command.parameters[2], -42);
	}
	else if (command.parameters[1][0] == '-')
	{
		channel->unset_limit();
		channel->broadcast(_nick + " MODE " + channel->get_name() + " -l" + " :limit removed", -42);
	}
}

void Client::handle_o_mode(t_command &command)
{
	if (command.parameters.size() > 2)
	{
		send_message(":ft_irc 461 * MODE :Too many parameters");
		std::cout << "too many parameters" << std::endl;
		return ;
	}
	Channel *channel = g_server.get_channels()[command.parameters[0]];
	if (!channel->is_user(_fd))
	{
		send_message(":ft_irc 442 * " + command.parameters[0] + " :You're not on that channel");
		std::cout << "not on that channel" << std::endl;
		return ;
	}
	if (!_is_operator)
	{
		send_message(":ft_irc 482 * " + command.parameters[0] + " :You're not operator");
		std::cout << "not operator" << std::endl;
		return ;
	}
	channel->set_operator_only(command.parameters[1][0] == '+');
	channel->broadcast(_nick + " MODE " + channel->get_name() + " +o" + " :is " + (channel->get_operator_only() ? "now" : "not anymore") + " operator only", -42);
}

int Client::command_MODE(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message(":ft_irc 461 * MODE :Not enough parameters");
		return (0);
	}
	if (command.parameters.size() == 1)
	{
		if (command.parameters[0][0] == '#')
		{
			if (g_server.get_channels().find(command.parameters[0]) == g_server.get_channels().end())
			{
				send_message(":ft_irc 403 * " + command.parameters[0] + " :No such channel");
				return (0);
			}
			else
			{
				send_message(":ft_irc 324 * " + command.parameters[0] + " +" 
				+ (g_server.get_channels()[command.parameters[0]]->get_invite_only() ? "i" : "") 
				+ (g_server.get_channels()[command.parameters[0]]->get_topic_set() ? "t" : "") 
				+ (g_server.get_channels()[command.parameters[0]]->get_key_set() ? "k" : "") 
				+ (g_server.get_channels()[command.parameters[0]]->get_limit_set() ? "l" : ""));
				return (0);
			}
		}
		else
		{
			send_message(":ft_irc 461 * MODE :Missing channel name");
			return (0);
		}
		return (0);
	}
	else
	{
		if (command.parameters[1][0] == '+' || command.parameters[1][0] == '-')
		{
			if (command.parameters[0][0] == '#')
			{
				if (g_server.get_channels().find(command.parameters[0]) == g_server.get_channels().end())
				{
					send_message(":ft_irc 403 * " + command.parameters[0] + " :No such channel");
					return (0);
				}
				Channel *channel = g_server.get_channels()[command.parameters[0]];
				if (!channel->is_user(_fd))
				{
					send_message(":ft_irc 442 * " + command.parameters[0] + " :You're not on that channel");
					return (0);
				}
				if (command.parameters[1].size() == 1)
				{
					send_message(":ft_irc 461 * MODE :Not enough parameters");
					return (0);
				}
				if (command.parameters[1][1] == 'i')
					handle_i_mode(command);
				else if (command.parameters[1][1] == 't')
					handle_t_mode(command);
				else if (command.parameters[1][1] == 'k')
					handle_k_mode(command);
				else if (command.parameters[1][1] == 'l')
					handle_l_mode(command);
				else if (command.parameters[1][1] == 'o')
					handle_o_mode(command);
				else
				{
					send_message(":ft_irc 461 * MODE :Invalid mode");
					return (0);
				}
				return (0);
			}
			if (command.parameters[1][1] == 'o')
			{
				if (g_server.find_client_by_nick(command.parameters[0]) == NULL)
				{
					send_message(":ft_irc 401 * " + command.parameters[0] + " :No such nick");
					return (0);
				}
				Client *client = g_server.find_client_by_nick(command.parameters[0]);
				if (!_is_operator)
				{
					send_message(":ft_irc 482 * " + command.parameters[0] + " :You're not operator");
					return (0);
				}
				client->set_operator(command.parameters[1][0] == '+');
				client->send_message(":ft_irc 381 * " + command.parameters[0] + " :You're now " + (client->is_operator() ? "an operator" : "not an operator"));
				return (0);
			}
			if (command.parameters[1][1] == 'i')
			{
				send_message(":ft_irc 381 * :You're now invisible");
				return (0);
			}
			else
			{
				send_message(":ft_irc 461 * MODE :Invalid channel name");
				return (0);
			}
		}
		else
		{
			send_message(":ft_irc 461 * MODE :Invalid mode");
			return (0);
		}
	}
	return (0);
}