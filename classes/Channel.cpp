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
}

Channel::Channel(std::string name, int fd_creator) {
    _name = name;
    _fds_users = std::vector<int>();
    _fds_users.push_back(fd_creator);
    _topic = "";
    _is_topic_set = false;
    _key = "";
    _max_users = 0;
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
        _fds_users.push_back(fd_user);
    else
        std::cerr << "User already in channel" << std::endl;
}

void Channel::part_user(int fd_user) {
	Client *leaving;

    if (!is_user(fd_user)) {
        std::cerr << "Couldn't part user: no such user in channel" << std::endl;
        return;
    }

	leaving = (*g_server.get_clients().find(fd_user)).second;
	broadcast(":ft_irc 381 " + _name + " :" + leaving->get_nick() + " has left the channel", fd_user);
    for (std::vector<int>::iterator it = _fds_users.begin(); it != _fds_users.end(); it++) {
        if (*it == fd_user) {
            _fds_users.erase(it);
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
	broadcast(":ft_irc 381 " + _name + " :" + kicked->get_nick() + " has been kicked from channel", fd_to_kick);
    for (std::vector<int>::iterator it = _fds_users.begin(); it != _fds_users.end(); it++) {
        if (*it == fd_to_kick) {
            _fds_users.erase(it);
            return;
        }
    }
    std::cerr << "Error while kicking user from channel" << std::endl;
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

void Channel::set_operator_only(bool is_operator_only) {
    _is_operator_only = is_operator_only;
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
    return _max_users;
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

bool Channel::get_operator_only(void) const {
    return _is_operator_only;
}

void Channel::broadcast(std::string message, int emitter) {
    for (std::vector<int>::iterator it = _fds_users.begin(); it != _fds_users.end(); it++) {
        if (*it != emitter) {
            g_server.get_clients()[*it]->send_message(message);
        }
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