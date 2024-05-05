/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:54:39 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/06 01:02:47 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"

std::ostream& operator<<(std::ostream &out, t_command &cmd)
{
    if (cmd.prefix != "")
		out << MAGENTA << "PREFIX = " << RESET << YELLOW << cmd.prefix << RESET << std::endl << std::endl << std::endl;
	else
		out << RED << "NO PREFIX" << RESET << std::endl;
    out    << MAGENTA << "COMMAND = " << RESET << GREEN << cmd.command << RESET << std::endl;
    out    << MAGENTA << "PARAMETERS :" << RESET << std::endl;
    for (std::vector<std::string>::iterator it = cmd.parameters.begin(); it != cmd.parameters.end(); it++) {
        out << "        - " << BLUE << *it << RESET << std::endl;
    }
	if (cmd.suffix != "")
		out << MAGENTA << "SUFFIX = " << RESET << YELLOW << cmd.suffix << RESET << std::endl << std::endl << std::endl;
	else
		out << RED << "NO SUFFIX" << RESET << std::endl;
    return (out);
}

t_command	parse_command(std::string &line)
{
	std::stringstream	ss(line);
	t_command			cmd;
	std::string			word;

	cmd.has_suffix = false;
	bool first = true;
	while (ss >> word)
	{
		if (first && word[0] == ':')
		{
			word.erase(0, 1);
			cmd.prefix = word;
			first = false;
			continue ;
		}
		if (first)
		{
			std::transform(word.begin(), word.end(), word.begin(), ::toupper);
			cmd.command = word;
			first = false;
			continue ;
		}
		if (word[0] == ':' || cmd.has_suffix)
		{
			cmd.has_suffix = true;
			if (word[0] == ':')
				word.erase(0, 1);
			if (cmd.suffix.size())
				cmd.suffix.append(" ");
			cmd.suffix.append(word);
			continue ;
		}
		cmd.parameters.push_back(word);
	}
	if (cmd.parameters.size())
	{
		std::stringstream s(cmd.parameters.at(0));
		while (getline(s, word, ','))
			cmd.cmds.push_back(word);
	}
	return (cmd);
}

void	handle_message(int fd)
{
	char	buf[BUFFER_SIZE + 1];
	ssize_t	rc;
	bool	closed;
	std::string line;

	closed = false;
	while (g_server.get_clients()[fd] && true)
	{
		rc = recv(fd, buf, BUFFER_SIZE, 0);
		if (rc <= 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Error: recv() failed: " << strerror(errno) << std::endl;
				closed = true;
			}
			std::stringstream ss(g_server.get_clients()[fd]->get_message());
			while (std::getline(ss, line))
			{
				// std::cout << "####  NEW COMMAND  ####" << std::endl;
				std::cout << line << std::endl;
				t_command cmd;
				if (line.size() == g_server.get_clients()[fd]->get_message().size())
					break ;
				cmd = parse_command(line);
				// std::cout << cmd << std::endl;
				if (g_server.get_commands().find(cmd.command) != g_server.get_commands().end())
				{
					if (cmd.cmds.size())
					{
						for (std::vector<std::string>::iterator it = cmd.cmds.begin(); it != cmd.cmds.end(); it++)
						{
							cmd.parameters.at(0) = *it;
							(g_server.get_clients()[fd]->*g_server.get_commands()[cmd.command])(cmd);
							if (!g_server.get_clients()[fd])
								return ;
						}
					}
					else 
						(g_server.get_clients()[fd]->*g_server.get_commands()[cmd.command])(cmd);
				}
				else
					(g_server.get_clients()[fd]->*g_server.get_commands()["UNKNOWN"])(cmd);
				if (g_server.get_clients()[fd])
					g_server.get_clients()[fd]->get_message().erase(0, line.size() + 1);
			}
			break ;
		}
		buf[rc] = 0;
		g_server.get_clients()[fd]->get_message().append(buf);
	}
}
