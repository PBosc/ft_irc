/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:54:39 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/03 04:14:29 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"

std::ostream& operator<<(std::ostream &out, t_command &cmd)
{
    out << "Le prefixe est " 
		<< cmd.prefix 
		<< ", la commande est " 
		<< cmd.command
		<< ", les parametres sont ";
	for (std::vector<std::string>::iterator it = cmd.parameters.begin(); it != cmd.parameters.end(); it++) {
		out << *it << " ";
	}
	out << "et le suffixe est "
		<< cmd.suffix;
    return (out);
}



bool format_command(std::string line, t_command &cmd) {
	std::stringstream ss(line);
	std::vector<std::string> all;
	std::string tmp;
	
	while (ss >> tmp) {
		// std::cout << "From stream is " << tmp << std::endl;
		all.push_back(tmp);
	}
	if (all.size() < 1)
		return false;
	std::vector<std::string>::iterator it = all.begin();
	if ((*it)[0] == ':') {
		(*it).erase(0, 1);	
		cmd.prefix = *(it++);
	}
	else
		cmd.prefix = "";
	if (it == all.end())
		return false;
	cmd.command = *(it++);
	while (it != all.end() && (*it)[0] != ':')
		cmd.parameters.push_back(*(it++));
	cmd.suffix = "";
	while (it != all.end())
		cmd.suffix += *(it++) + " ";
	if (cmd.suffix != "")
		cmd.suffix.erase(0, 1);
	return true;
}

void	handle_message(int fd)
{
	char	buf[BUFFER_SIZE + 1];
	ssize_t	rc;
	bool	closed;
	Client	*client;
	std::string line;

	client = g_server.get_clients()[fd];
	std::cout << "User " << fd << " is sending a command" << std::endl;
	closed = false;
	while (true)
	{
		rc = recv(fd, buf, BUFFER_SIZE, 0);
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Error: recv() failed: " << strerror(errno) << std::endl;
				closed = true;
			}
			std::stringstream ss(client->get_message());
			while (std::getline(ss, line))
			{
				t_command cmd;
				if (line.size() == client->get_message().size())
					break ;
				if (!format_command(line, cmd)) {
					std::cout << "Failure" << std::endl;
					// todo error handling if command is not correct format 
				}
				if (g_server.get_commands().find(cmd.command) != g_server.get_commands().end())
					(client->*g_server.get_commands()[cmd.command])(cmd);
				else
					(client->*g_server.get_commands()["UNKNOWN"])(cmd);
				std::cout << cmd << std::endl;
				client->get_message().erase(0, line.size() + 1);
			}
			break ;
		}
		buf[rc] = 0;
		client->get_message().append(buf);
	}
}
