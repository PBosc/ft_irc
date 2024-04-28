#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "IRC.hpp"
# include "Channel.hpp"

class Client
{
	private:
		int						_id, _fd;
		std::string				_nick, _Client, _name;
		bool					_has_password, _has_nick, _has_Client, _is_identified;
		std::vector<Channel *>	_channels;

	public:
		Client();
		Client(int fd, int id);
		Client(const  Client &obj);
		Client& operator=(const Client &rhs);
		~Client();
		bool	send_message(const std::string &message);
		//void	execute_commands(t_command &command);
		//bool	command_PASS(t_command &command);
		//bool	command_NICK(t_command &command);
		//bool	command_Client(t_command &command);
		//bool	command_PING(t_command &command);
		//bool	command_JOIN(t_command &command);
		//bool	command_names(t_command &command);
		//bool	command_PRIVMSG(t_command &command);
		//int		command_QUIT(t_command &command);
		//bool	command_PART(t_command &command);
		//bool	command_KICK(t_command &command);
		//int		command_KILL(t_command &command);
		//bool	command_OPER(t_command &command);
		//bool	command_MODE(t_command &command);
		//bool	command_TOPIC(t_command &command);
		//bool	command_NOTICE(t_command &command);
		//bool	command_INVITE(t_command &command);
		//bool	command_unknown(t_command &command);

		bool		remove_channel(std::string channel_name);
		static Client *getClient(std::string nick, t_data *server);
		int			get_id(void) const;
		int			get_fd(void) const;
		std::string	get_nick(void) const;
		std::string	get_Client(void) const;
		bool		get_identification(void) const;
		bool    	is_operator(void);
};

#endif