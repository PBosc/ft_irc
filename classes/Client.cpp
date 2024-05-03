#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"


Client::Client()
{
	_id = -1;
	_fd = -1;
	_nick = "";
	_user = "";
	_name = "";
	_has_password = false;
	_has_nick = false;
	_has_Client = false;
	_is_identified = false;
	_message = "";
}

Client::Client(int fd, int id)
{
	_id = id;
	_fd = fd;
	_nick = "";
	_user = "";
	_name = "";
	_has_password = false;
	_has_nick = false;
	_has_Client = false;
	_is_identified = false;
	_message = "";
}

Client::Client(const Client &obj)
{
	_id = obj._id;
	_fd = obj._fd;
	_nick = obj._nick;
	_user = obj._user;
	_name = obj._name;
	_has_password = obj._has_password;
	_has_nick = obj._has_nick;
	_has_Client = obj._has_Client;
	_is_identified = obj._is_identified;
	_message = obj._message;
}

Client &Client::operator=(const Client &rhs)
{
	_id = rhs._id;
	_fd = rhs._fd;
	_nick = rhs._nick;
	_user = rhs._user;
	_name = rhs._name;
	_has_password = rhs._has_password;
	_has_nick = rhs._has_nick;
	_has_Client = rhs._has_Client;
	_is_identified = rhs._is_identified;
	_message = rhs._message;
	return (*this);
}

bool Client::send_message(const std::string &message)
{
	return (send(_fd, (message + "\n").c_str(), message.length() + 1, 0) != -1);
}

bool Client::remove_channel(std::string channel_name)
{
	for (std::map<std::string, Channel *>::iterator it = g_server.get_channels().begin(); it != g_server.get_channels().end(); it++)
	{
		if ((*it).second->is_user(_fd))
		{
			(*it).second->part_user(_fd);
			return (true);
		}
	}
	std::cerr << "Couldn't remove channel " << channel_name << " from client " << _id << ": no such channel" << std::endl;
	return (false);
}

int Client::get_id(void) const
{
	return (_id);
}

int Client::get_fd(void) const
{
	return (_fd);
}

std::string Client::get_nick(void) const
{
	return (_nick);
}

std::string Client::get_client(void) const
{
	return (_user);
}

bool Client::get_identification(void) const
{
	return (_is_identified);
}

bool Client::is_operator(void)
{
	return (_is_operator);
}

std::string &Client::get_message(void)
{
	return (_message);
}

void Client::set_message(std::string &message)
{
	_message = message;
}

std::ostream &operator<<(std::ostream &os, const Client &client)
{
	os << "Client " << client.get_id() << " (" << client.get_nick() << ")"
		<< "writing to fd " << client.get_fd() << std::endl;
	return (os);
}

int Client::command_CAP(t_command &command)
{
    if (command.parameters.size() < 1)
    {
        send_message("ERROR :No capability given");
        return (0);
    }
    if (command.parameters[0] == "LS")
        send_message("CAP * LS :cap1 cap2 cap3");
    else if (command.parameters[0] == "END")
	{
		if (!_has_password)
		{
			send_message("ERROR :You must set your password before ending capabilities");
			return (0);
		}
		if (!_has_nick)
		{
			send_message("ERROR :You must set your nick before ending capabilities");
			return (0);
		}
		if (!_has_Client)
		{
			send_message("ERROR :You must set your Client before ending capabilities");
			return (0);
		}
		send_message(":ft_irc 001 " + _nick + " :Welcome to the IRC network, " + _nick + "!");
	}
    else
		send_message("ERROR :Invalid capability");
	return (0);
}

int Client::command_PASS(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No password given");
		return (0);
	}
	if (_has_password)
	{
		send_message("ERROR :You have already set your password");
		return (0);
	}
	if (command.parameters[0] != g_server.get_password())
	{
		send_message("ERROR :Invalid password");
		return (0);
	}
	_has_password = true;
	return (0);
}

bool erroneous_nick(std::string nick) {
	std::string specials = "[]|^_-{}";
	for (std::string::iterator it = nick.begin(); it != nick.end(); it++) {
		if (!isalnum(*it) && specials.find(*it) == std::string::npos)
			return true;
	}
	return false;
}

bool is_already_in_use(std::string nick) {
	for (std::map<int, Client *>::iterator it = g_server.get_clients().begin(); it != g_server.get_clients().end(); it++) {
		if (nick == (*it).second->get_nick())
			return true;
	}
	return false;
}

int Client::command_NICK(t_command &cmd)
{
	std::string old_nick = _nick;

	if (cmd.parameters.size() < 1)
	{
		send_message(":ft_irc 421 * " + _nick + " NICK :Command requires one argument");
		return 0;
	}
	if (cmd.parameters.size() > 1)
	{
		send_message(":ft_irc 421 * " + _nick + " NICK :Command takes only one argument");
		return 0;
	}
	if (erroneous_nick(cmd.parameters[0])) {
		send_message(":ft_irc 432 * " + cmd.parameters[0] + " :Erroneous nickname");
		return 0;
	}
	// if (is_already_in_use(cmd.parameters[0])) {
	// 	send_message(":ft_irc 433 * " + cmd.parameters[0] + " :Nickname is already in use");
	// 	return 0;
	// }

	_nick = cmd.parameters[0];

	send_message(":" + old_nick + "!" + _user + "@ft_irc NICK " + _nick);
	_has_nick = true;
	return 1;
}

int Client::command_USER(t_command &command)
{
	if (_has_Client)
	{
		send_message("ERROR :You have already set your Client");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No Client given");
		return (0);
	}
	_user = command.parameters[0];
	_has_Client = true;
	return (0);
}

int Client::command_PING(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No PING message given");
		return (0);
	}
	send_message("PONG " + command.parameters[0]);
	return (0);
}

int Client::command_JOIN(t_command &command)
{
    Channel    *channel;

    if (command.parameters.size() < 1)
    {
        send_message("ERROR :No channel given");
        return (0);
    }
    if (g_server.get_channels().find(command.parameters[0]) == g_server.get_channels().end())
        g_server.get_channels()[command.parameters[0]] = new Channel(command.parameters[0], _fd);
    channel = g_server.get_channels()[command.parameters[0]];
    if (channel->is_user(_fd))
    {
        send_message("ERROR :You are already in channel "
            + command.parameters[0]);
        return (0);
    }
    if (channel->get_user_limit() != 0
        && channel->get_users().size() >= channel->get_user_limit())
    {
        send_message("ERROR :Channel " + command.parameters[0] + " is full");
        return (0);
    }
    channel->add_user(_fd);
    return (0);
}

int Client::command_NAMES(t_command &command)
{
	Channel	*channel;

	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No channel given");
		return (0);
	}
    if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
        channel = g_server.get_channels()[command.parameters[0]];
    else
	{
    	send_message("ERROR :No such channel " + command.parameters[0]);
		return (0);
    }
	std::string names = "NAMES " + command.parameters[0] + " :";
	for (std::size_t i = 0; i < channel->get_users().size(); ++i)
	{
		// int client_fd = channel->get_users().at(i);
		// Client *client = g_data->clients[client_fd];
	}
	send_message(names);
	return (0);
}

int Client::command_PRIVMSG(t_command &command)
{
    Channel    *channel;

    std::cout << "PRIVMSG command" << std::endl;
    if (command.parameters.size() < 1)
    {
        send_message("ERROR :Not enough parameters");
        std::cout << "Not enough parameters" << std::endl;
        return (0);
    }
    if (command.parameters[0][0] == '#')
    {
        if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
            channel = g_server.get_channels()[command.parameters[0]];
        else
        {
            send_message("ERROR :No such channel " + command.parameters[0]);
            std::cout << "No such channel" << std::endl;
            return (0);
        }
        if (!channel->is_user(_fd))
        {
            send_message("ERROR :You are not in channel "
                + command.parameters[0]);
            std::cout << "You are not in channel" << std::endl;
            return (0);
        }
        std::string message = ":" + g_server.get_clients()[_fd]->get_nick() + " PRIVMSG " + command.parameters[0] + " :"
            + command.suffix;
        std::cout << "Broadcasting message: " << std::endl;
        channel->broadcast(message, _fd);
    }
    return (0);
}

int Client::command_QUIT(t_command &command)
{
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No quit message given");
		return 0;
	}
	send_message("ERROR :Closing connection: " + command.parameters[0]);
	return 0;
}

int Client::command_PART(t_command &command)
{
	Channel	*channel;

	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No channel given");
		return 0;
	}
    if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
        channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message("ERROR :No such channel " + command.parameters[0]);
		return 0;
	}
	if (!channel->is_user(_fd))
	{
		send_message("ERROR :You are not in channel " + command.parameters[0]);
		return 0;
	}
	channel->part_user(_fd);
	return 0;
}

int Client::command_KICK(t_command &command)
{
	Channel	*channel;

	if (command.parameters.size() < 2)
	{
		send_message("ERROR :Not enough parameters");
		return 0;
	}
	if (!_is_operator)
	{
		send_message("ERROR :You are not an operator");
		return 0;
	}
    if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
        channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message("ERROR :No such channel " + command.parameters[0]);
		return 0;
	}
	if (!channel->is_user(_fd))
	{
		send_message("ERROR :You are not in channel " + command.parameters[0]);
		return 0;
	}
	return 0;
}

int Client::command_KILL(t_command &command)
{
	if (command.parameters.size() < 2)
	{
		send_message("ERROR :Not enough parameters");
		return 0;
	}
	if (!_is_operator)
	{
		send_message("ERROR :You are not an operator");
		return 0;
	}
	return 0;
}

int Client::command_OPER(t_command &command)
{
	if (command.parameters.size() < 2)
	{
		send_message("ERROR :Not enough parameters");
		return 0;
	}
	if (_has_password)
	{
		send_message("ERROR :You have already set your password");
		return 0;
	}
	if (command.parameters[1] != "password")
	{
		send_message("ERROR :Invalid password");
		return 0;
	}
	_has_password = true;
	_is_operator = true;
	return 0;
}

int Client::command_MODE(t_command &command)
{
	(void)command;
	send_message("ERROR :MODE command not implemented");
	return 0;
}

int Client::command_TOPIC(t_command &command)
{
	Channel	*channel;

	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No channel given");
		return 0;
	}
    if (g_server.get_channels().find(command.parameters[0]) != g_server.get_channels().end())
        channel = g_server.get_channels()[command.parameters[0]];
	else
	{
		send_message("ERROR :No such channel " + command.parameters[0]);
		return 0;
	}
	if (command.parameters.size() < 2)
	{
		send_message("ERROR :No topic given");
		return 0;
	}
	if (!channel->is_user(_fd))
	{
		send_message("ERROR :You are not in channel " + command.parameters[0]);
		return 0;
	}
	channel->set_topic(command.parameters[1]);
	return 0;
}

int Client::command_NOTICE(t_command &command)
{
	(void)command;
	send_message("ERROR :NOTICE command not implemented");
	return 0;
}

int Client::command_INVITE(t_command &command)
{
	(void)command;
	send_message("ERROR :INVITE command not implemented");
	return 0;
}

int Client::command_unknown(t_command &command)
{
	send_message("ERROR :Unknown command " + command.command);
	return 0;
}