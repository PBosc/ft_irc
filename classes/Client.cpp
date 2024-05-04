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

bool Client::can_execute(void) const
{
	return (_has_password && _has_nick && _has_Client);
}

bool Client::send_message(const std::string &message)
{
	int		rc;
	try {
		rc = send(_fd, (message + "\r\n").c_str(), message.length() + 2, 0);
		return (true);
	}
	catch (std::exception &e) {
		std::cerr << "Error: send() failed: " << e.what() << std::endl;
		return (false);
	}
}

bool Client::remove_channel(std::string channel_name)
{
	for (std::map<std::string,
		Channel *>::iterator it = g_server.get_channels().begin(); it != g_server.get_channels().end(); it++)
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

void Client::set_operator(bool b) {
	_is_operator = b;
}

std::ostream &operator<<(std::ostream &os, const Client &client)
{
	os << "Client " << client.get_id() << " (" << client.get_nick() << ")"
		<< "writing to fd " << client.get_fd() << std::endl;
	return (os);
}