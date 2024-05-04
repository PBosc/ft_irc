/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pibosc <pibosc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:54:39 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/04 03:12:59 by pibosc           ###   ########.fr       */
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

	ss >> word;
	unsigned long pos = word.find(":");
	while (pos == word.size() - 1)
	{
		cmd.prefix.append(word);
		ss >> word;
		pos = word.find(":");
	}
	cmd.command = word;
	while (ss >> word && word[0] != ':')
	{
		cmd.parameters.push_back(word);
	}
	bool first = true;
	while (ss)
	{
		if (!first)
			cmd.suffix.append(" ");
		first = false;
		if (word[0] == ':')
			word.erase(0, 1);
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
	Client	*client;
	std::string line;

	client = g_server.get_clients()[fd];
	closed = false;
	while (true)
	{
		rc = recv(fd, buf, BUFFER_SIZE, 0);
		if (rc <= 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Error: recv() failed: " << strerror(errno) << std::endl;
				closed = true;
			}
			std::stringstream ss(client->get_message());
			while (std::getline(ss, line))
			{
				std::cout << line << std::endl;
				t_command cmd;
				if (line.size() == client->get_message().size())
					break ;
				cmd = parse_command(line);
				std::cout << cmd << std::endl;
				if (g_server.get_commands().find(cmd.command) != g_server.get_commands().end())
				{
					if ((client->*g_server.get_commands()[cmd.command])(cmd))
						return ;
				}
				else
					(client->*g_server.get_commands()["UNKNOWN"])(cmd);
				client->get_message().erase(0, line.size() + 1);
			}
			break ;
		}
		buf[rc] = 0;
		client->get_message().append(buf);
	}
}