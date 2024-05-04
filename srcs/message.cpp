/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:54:39 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/04 21:34:58 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"

std::ostream& operator<<(std::ostream &out, t_command &cmd)
{
    out << "####  NEW COMMAND  ####" << std::endl;
    out << "    PREFIX = " << cmd.prefix << std::endl; 
    out    << "    COMMAND = " << cmd.command << std::endl;
    out    << " PARAMETERS :" << std::endl;
    for (std::vector<std::string>::iterator it = cmd.parameters.begin(); it != cmd.parameters.end(); it++) {
        out << "        - " << *it << std::endl;
    }
    out << "    SUFFIX = " << cmd.suffix << std::endl << std::endl << std::endl;
    return (out);
}

t_command parse_command(std::string line)
{
	t_command cmd;
	
	std::stringstream ss(line);
	std::string word;

	cmd.has_suffix = false;
	ss >> word;
	unsigned long pos = word.find(":");
	while (pos == word.size() - 1)
	{
		cmd.prefix.append(word);
		ss >> word;
		pos = word.find(":");
	}
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	cmd.command = word;
	while (ss >> word && word[0] != ':')
	{
		cmd.parameters.push_back(word);
	}
	bool first = true;
	while (ss)
	{
		cmd.has_suffix = true;
		if (!first)
			cmd.suffix.append(" ");
		if (first && word[0] == ':')
			word.erase(0, 1);
		first = false;
		cmd.suffix.append(word);
		ss >> word;
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
				std::cout << line << std::endl;
				t_command cmd;
				if (line.size() == g_server.get_clients()[fd]->get_message().size())
					break ;
				cmd = parse_command(line);
				std::cout << cmd << std::endl;
				if (g_server.get_commands().find(cmd.command) != g_server.get_commands().end())
				{
					if ((g_server.get_clients()[fd]->*g_server.get_commands()[cmd.command])(cmd))
						return ;
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
