#include "Channel.hpp"
#include "Client.hpp"

void Client::init_commands(void)
{
	commands["PASS"] = &Client::command_PASS;
	commands["NICK"] = &Client::command_NICK;
	commands["USER"] = &Client::command_USER;
	commands["PING"] = &Client::command_PING;
	commands["JOIN"] = &Client::command_JOIN;
	commands["NAMES"] = &Client::command_NAMES;
	commands["PRIVMSG"] = &Client::command_PRIVMSG;
	commands["QUIT"] = &Client::command_QUIT;
	commands["PART"] = &Client::command_PART;
	commands["KICK"] = &Client::command_KICK;
	commands["KILL"] = &Client::command_KILL;
	commands["OPER"] = &Client::command_OPER;
	commands["MODE"] = &Client::command_MODE;
	commands["TOPIC"] = &Client::command_TOPIC;
	commands["NOTICE"] = &Client::command_NOTICE;
	commands["INVITE"] = &Client::command_INVITE;
	commands["UNKNOWN"] = &Client::command_unknown;
}

Client::Client()
{
	_id = -1;
	_fd = -1;
	_nick = "";
	_client = "";
	_name = "";
	_has_password = false;
	_has_nick = false;
	_has_Client = false;
	_is_identified = false;
	_channels = std::vector<Channel *>();
	_message = "";
	init_commands();
}

Client::Client(int fd, int id)
{
	_id = id;
	_fd = fd;
	_nick = "";
	_client = "";
	_name = "";
	_has_password = false;
	_has_nick = false;
	_has_Client = false;
	_is_identified = false;
	_channels = std::vector<Channel *>();
	_message = "";
	init_commands();
}

Client::Client(const Client &obj)
{
	_id = obj._id;
	_fd = obj._fd;
	_nick = obj._nick;
	_client = obj._client;
	_name = obj._name;
	_has_password = obj._has_password;
	_has_nick = obj._has_nick;
	_has_Client = obj._has_Client;
	_is_identified = obj._is_identified;
	_channels = obj._channels;
	_message = obj._message;
	commands = obj.commands;
}

Client &Client::operator=(const Client &rhs)
{
	_id = rhs._id;
	_fd = rhs._fd;
	_nick = rhs._nick;
	_client = rhs._client;
	_name = rhs._name;
	_has_password = rhs._has_password;
	_has_nick = rhs._has_nick;
	_has_Client = rhs._has_Client;
	_is_identified = rhs._is_identified;
	_channels = rhs._channels;
	_message = rhs._message;
	return (*this);
}

Client::~Client()
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		delete *it;
	}
}

bool Client::send_message(const std::string &message)
{
	return (send(_fd, message.c_str(), message.length(), 0) != -1);
}

bool Client::remove_channel(std::string channel_name)
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->get_name() == channel_name)
		{
			_channels.erase(it);
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
	return (_client);
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

int Client::command_PASS(t_command &)
{
	return (0);
}

int Client::command_NICK(t_command &command)
{
	if (_has_nick)
	{
		send_message("ERROR :You have already set your nickname");
		return (0);
	}
	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No nickname given");
		return (0);
	}
	_nick = command.parameters[0];
	_has_nick = true;
	return (0);
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
	_client = command.parameters[0];
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
	Channel	*channel;

	if (command.parameters.size() < 1)
	{
		send_message("ERROR :No channel given");
		return (0);
	}
	if (!_is_identified)
	{
		send_message("ERROR :You have not registered");
		return (0);
	}
	channel = Channel::getChannel(command.parameters[0]);
	if (channel == NULL)
	{
		channel = new Channel(command.parameters[0], _fd);
		_channels.push_back(channel);
	}
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
	channel = Channel::getChannel(command.parameters[0]);
	if (channel == NULL)
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
	Channel	*channel;

	if (command.parameters.size() < 2)
	{
		send_message("ERROR :Not enough parameters");
		return (0);
	}
	if (command.parameters[0][0] == '#')
	{
		channel = Channel::getChannel(command.parameters[0]);
		if (channel == NULL)
		{
			send_message("ERROR :No such channel " + command.parameters[0]);
			return (0);
		}
		if (!channel->is_user(_fd))
		{
			send_message("ERROR :You are not in channel "
				+ command.parameters[0]);
			return (0);
		}
		std::string message = "PRIVMSG " + command.parameters[0] + " :"
			+ command.parameters[1];
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
	channel = Channel::getChannel(command.parameters[0]);
	if (channel == NULL)
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
	channel = Channel::getChannel(command.parameters[0]);
	if (channel == NULL)
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
	channel = Channel::getChannel(command.parameters[0]);
	if (channel == NULL)
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