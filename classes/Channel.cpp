#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

Channel::Channel() {
    _name = "";
    _fds_users = std::vector<int>();
    _topic = "";
    _is_topic_set = false;
    _key = "";
    _max_users = 0;
    _is_invite_only = false;
    _is_key_set = false;
    _is_topic_op_only = false;
    _is_limit_set = false;
    _limit = 0;
}

Channel::Channel(std::string name) {
    _name = name;
    _fds_users = std::vector<int>();
    _topic = "";
    _is_topic_set = false;
    _key = "";
    _max_users = 0;
    _is_invite_only = false;
    _is_key_set = false;
    _is_topic_op_only = false;
    _is_limit_set = false;
    _limit = 0;
}

Channel::~Channel() { }

bool Channel::is_user(int fd_user) {
    for (std::vector<int>::iterator it = _fds_users.begin(); it != _fds_users.end(); it++) {
        if (*it == fd_user) {
            return true;
        }
    }
    return false;
}

void Channel::add_user(int fd_user) {
    if (!is_user(fd_user))
	{
        _fds_users.push_back(fd_user);
		_fds_operators[fd_user] = false;
	}
    else
        std::cerr << "User already in channel" << std::endl;
}

void Channel::part_user(int fd_user, const std::string &reason, bool iterator) {
	Client *leaving;

    if (!is_user(fd_user)) {
        std::cerr << "Couldn't part user: no such user in channel" << std::endl;
        return;
    }

	leaving = (*g_server.get_clients().find(fd_user)).second;
    if (!leaving)
    {
        std::cerr << "Couldn't part user: no such user in server" << std::endl;
        return;
    }
    std::string message = ":" + leaving->get_nick() + "!" + leaving->get_client() + "@" + leaving->get_addr() + " PART " + _name;
    if (!reason.empty())
    {
        message += " :" + reason;
    }
	broadcast(message, -42);
    for (std::vector<int>::iterator it = _fds_users.begin(); it != _fds_users.end(); it++) {
        if (*it == fd_user) {

            _fds_users.erase(it);
			if (_fds_users.size() == 0) {
                if (!iterator)
                {
                    g_server.get_channels().erase(_name);
                    delete this;
                }
				return ;
			}
			if (_fds_operators[fd_user] && op_count() == 1) {
				_fds_operators[_fds_users.at(0)] = true;
				Client	*client = g_server.get_clients()[_fds_users.at(0)];
				client->send_message(":" + client->get_server_addr() + " MODE " + _name + " +o :" + client->get_nick() + " is now an operator");
				_fds_operators.erase(fd_user);
			}
            return;
        }
    }
    std::cerr << "Error while parting user from channel" << std::endl;
}

void Channel::kick_user(int fd_to_kick) {
	Client *kicked;

    if (!is_user(fd_to_kick)) {
        std::cerr << "Couldn't kick user: no such user in channel" << std::endl;
        return;
    }

	kicked = (*g_server.get_clients().find(fd_to_kick)).second;
    if (!kicked)
    {
        std::cerr << "Couldn't kick user: no such user in server" << std::endl;
        return;
    }
    std::string message(":" + kicked->get_server_addr() + " 381 " + _name + " :" + kicked->get_nick() + " has been kicked from channel");
	broadcast(message, fd_to_kick);
    for (std::vector<int>::iterator it = _fds_users.begin(); it != _fds_users.end(); it++) {
        if (*it == fd_to_kick) {

            _fds_users.erase(it);
			if (_fds_users.size() == 0) {
				return ;
			}
			if (_fds_operators[fd_to_kick] && op_count() == 1) {
				_fds_operators[_fds_users.at(0)] = true;
				Client	*client = g_server.get_clients()[_fds_users.at(0)];
				client->send_message(":" + client->get_server_addr() + " MODE " + _name + " +o :" + client->get_nick() + " is now an operator");
				_fds_operators.erase(fd_to_kick);
			}
            return;
        }
    }
    std::cerr << "Error while kicking user from channel" << std::endl;
}

int Channel::op_count() {
	int c = 0;
	for (std::map<int, bool>::iterator it = _fds_operators.begin(); it != _fds_operators.end(); ++it) {
		if (it->second)
			++c;
	}
	return c;
}

void Channel::invite(int fd_invited, Client *client)
{
	_invited_users[fd_invited] = client;
}


void Channel::set_topic(std::string topic) {
    _topic = topic;
    _is_topic_set = true;
}

void Channel::set_topic_op_only(bool is_topic_op_only) {
    _is_topic_op_only = is_topic_op_only;
}

void Channel::unset_topic() {
    _topic = "";
    _is_topic_set = false;
}

void Channel::set_max_users(int max_users) {
    _max_users = max_users;
}

void Channel::set_invite_only(bool is_invite_only) {
    _is_invite_only = is_invite_only;
}

void Channel::set_key(std::string key) {
    _key = key;
    _is_key_set = true;
}

void Channel::unset_key() {
    _key = "";
    _is_key_set = false;
}

void Channel::set_limit(unsigned int limit) {
    _limit = limit;
    _is_limit_set = true;
}

void Channel::unset_limit() {
    _limit = 0;
    _is_limit_set = false;
}

std::string Channel::get_name(void) const {
    return _name;
}

std::vector<int> Channel::get_users(void) const {
    return _fds_users;
}

std::string Channel::get_topic(void) const {
    return _topic;
}

bool Channel::get_topic_set(void) const {
    return _is_topic_set;
}

std::string Channel::get_key(void) const {
    return _key;
}

unsigned int Channel::get_user_limit(void) const {
    return _limit;
}

bool Channel::get_invite_only(void) const {
    return _is_invite_only;
}

bool Channel::get_key_set(void) const {
    return _is_key_set;
}

bool Channel::get_limit_set(void) const {
    return _is_limit_set;
}

bool Channel::get_topic_op_only(void) const {
    return _is_topic_op_only;
}

void Channel::set_oper(int fd, bool oper)
{
	_fds_operators[fd] = oper;
}

bool Channel::get_oper(int fd)
{
	return (_fds_operators[fd]);
}

std::map<int, Client *> &Channel::get_invited() {
	return _invited_users;
}

void Channel::broadcast(std::string &message, int emitter) {
    for (std::vector<int>::iterator it = _fds_users.begin(); it != _fds_users.end(); it++) {
        if (*it != emitter && g_server.get_clients()[*it]) {
            g_server.get_clients()[*it]->send_message(message);
        }
    }
    if (g_server.get_bot_fd() != -1 && g_server.get_bot_fd() != emitter) {
        g_server.get_clients()[g_server.get_bot_fd()]->send_message(message);
    }
}

std::ostream& operator<< (std::ostream &out, const Channel &channel) {
    out << "Channel " << channel.get_name() << ":\n";
    out << "Users:\n";
    for (std::size_t i = 0; i < channel.get_users().size(); ++i)
        out << channel.get_users().at(i);
    out << "Topic: " << channel.get_topic() << std::endl;
    out << "Topic set: " << channel.get_topic_set() << std::endl;
    out << "Key: " << channel.get_key() << std::endl;
    out << "User limit: " << channel.get_user_limit() << std::endl;
    return out;
}