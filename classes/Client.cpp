#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

Client::Client(int fd, int id, struct sockaddr_in socket)
{
	_id = id;
	_fd = fd;
	_nick = "";
	_user = "";
	_name = "";
	_real_name = "";
	_has_password = false;
	_has_nick = false;
	_has_Client = false;
	_is_identified = false;
	_is_operator = false;
	_is_invisible = false;
	_message = "";
	_socket = socket;
}

bool Client::can_execute(void) const
{
	return (_has_password && _has_nick && _has_Client);
}

bool Client::send_message(const std::string &message)
{
	if (message.length() >= 510)
		return (send(_fd, (message.substr(0, 510) + "\r\n").c_str(), 512, MSG_NOSIGNAL));
	return (send(_fd, (message + "\r\n").c_str(), message.length() + 2, MSG_NOSIGNAL) >= 0);
}

bool Client::remove_channel(std::string channel_name)
{
	for (std::map<std::string,
		Channel *>::iterator it = g_server.get_channels().begin(); it != g_server.get_channels().end(); it++)
	{
		if ((*it).second->is_user(_fd))
		{
			(*it).second->part_user(_fd, "", true);
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

std::string Client::get_name(void) const {return _name;}

std::string Client::get_real_name(void) const {return _real_name;}

std::string Client::get_addr(void) const
{
	return (inet_ntoa(_socket.sin_addr));
}

std::string Client::get_server_addr(void) const
{
	struct sockaddr_in local_sin;
    socklen_t local_sinlen = sizeof(local_sin);
	getsockname(_fd, (struct sockaddr*)&local_sin, &local_sinlen);
	return (inet_ntoa(local_sin.sin_addr));
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

bool Client::is_invisible(void) const
{
	return (_is_invisible);
}

void Client::set_invisible(bool b)
{
	_is_invisible = b;
}

std::ostream &operator<<(std::ostream &os, const Client &client)
{
	os << "Client " << client.get_id() << " (" << client.get_nick() << ")"
		<< "writing to fd " << client.get_fd() << std::endl;
	return (os);
}

bool	is_already_in_use(std::string nick)
{
	for (std::map<int,
		Client *>::iterator it = g_server.get_clients().begin(); it != g_server.get_clients().end(); it++)
	{
		if (!(*it).second)
			continue;
		if (nick == (*it).second->get_nick())
			return (true);
	}
	return (false);
}