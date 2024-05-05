#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Channel.hpp"
# include "IRC.hpp"

class Client
{
  private:
	int _id, _fd;
	std::string _nick, _user, _name;
	struct sockaddr_in	_socket;
	bool _has_password, _has_nick, _has_Client, _is_identified, _is_operator, _is_invisible;
	std::string _message;

  public:
	Client(int fd, int id, struct sockaddr_in socket);
	Client(const Client &obj);
	Client &operator=(const Client &rhs);
	bool send_message(const std::string &message);
	int command_CAP(t_command &command);
	int command_PASS(t_command &command);
	int command_NICK(t_command &command);
	int command_USER(t_command &command);
	int command_PING(t_command &command);
	int command_JOIN(t_command &command);
	int command_NAMES(t_command &command);
	int command_PRIVMSG(t_command &command);
	int command_QUIT(t_command &command);
	int command_PART(t_command &command);
	int command_KICK(t_command &command);
	int command_KILL(t_command &command);
	int command_OPER(t_command &command);
	int command_MODE(t_command &command);
	int command_TOPIC(t_command &command);
	int command_NOTICE(t_command &command);
	int command_INVITE(t_command &command);
	int command_unknown(t_command &command);

	void handle_i_mode(t_command &command, bool sign, Channel *channel);
	void handle_t_mode(t_command &command, bool sign, Channel *channel);
	void handle_k_mode(t_command &command, bool sign, size_t &params, Channel *channel);
	void handle_o_mode(t_command &command, bool sign, size_t &params, Channel *channel);
	void handle_l_mode(t_command &command, bool sign, size_t &params, Channel *channel);

	bool can_execute(void) const;

	bool remove_channel(std::string channel_name);
	int get_id(void) const;
	int get_fd(void) const;
	std::string get_nick(void) const;
	std::string get_client(void) const;
	std::string get_server_addr(void) const;
	std::string get_addr(void) const;
	bool get_identification(void) const;
	bool is_operator(void);
	std::string &get_message(void);
	void set_operator(bool b);
	void set_message(std::string &message);
	bool is_invisible(void) const;
	void set_invisible(bool b);
};

bool	is_already_in_use(std::string nick);

#endif