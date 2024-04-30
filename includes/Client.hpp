#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "IRC.hpp"
# include "Channel.hpp"


class Client
{
	typedef int(Client::*ScriptFunction)(t_command &command);

	private:
		int						_id, _fd;
		std::string				_nick, _client, _name;
		bool					_has_password, _has_nick, _has_Client, _is_identified, _is_operator;
		std::vector<Channel *>	_channels;
	public:
		Client();
		Client(int fd, int id);
		Client(const  Client &obj);
		Client& operator=(const Client &rhs);
		~Client();
		bool	send_message(const std::string &message);
		//void	execute_commands(t_command &command);
		int	command_PASS(t_command &command);
		int	command_NICK(t_command &command);
		int	command_USER(t_command &command);
		int	command_PING(t_command &command);
		int	command_JOIN(t_command &command);
		int	command_NAMES(t_command &command);
		int	command_PRIVMSG(t_command &command);
		int	command_QUIT(t_command &command);
		int	command_PART(t_command &command);
		int	command_KICK(t_command &command);
		int	command_KILL(t_command &command);
		int	command_OPER(t_command &command);
		int	command_MODE(t_command &command);
		int	command_TOPIC(t_command &command);
		int	command_NOTICE(t_command &command);
		int	command_INVITE(t_command &command);
		int	command_unknown(t_command &command);

		bool		remove_channel(std::string channel_name);
		static Client *getClient(std::string nick, t_data *server);
		int			get_id(void) const;
		int			get_fd(void) const;
		std::string	get_nick(void) const;
		std::string	get_client(void) const;
		bool		get_identification(void) const;
		bool    	is_operator(void);
		std::map<std::string, ScriptFunction>	commands = {
			{"PASS", &Client::command_PASS},
			{"NICK", &Client::command_NICK},
			{"USER", &Client::command_USER},
			{"PING", &Client::command_PING},
			{"JOIN", &Client::command_JOIN},
			{"NAMES", &Client::command_NAMES},
			{"PRIVMSG", &Client::command_PRIVMSG},
			{"QUIT", &Client::command_QUIT},
			{"PART", &Client::command_PART},
			{"KICK", &Client::command_KICK},
			{"KILL", &Client::command_KILL},
			{"OPER", &Client::command_OPER},
			{"MODE", &Client::command_MODE},
			{"TOPIC", &Client::command_TOPIC},
			{"NOTICE", &Client::command_NOTICE},
			{"INVITE", &Client::command_INVITE},
			{"UNKNOWN", &Client::command_unknown}
		};
};

#endif