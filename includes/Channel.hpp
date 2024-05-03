#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "IRC.hpp"

class Channel
{
	private:
		std::string			_name;
		std::vector<int>	_fds_users;
		std::string			_topic;
		bool				_is_topic_set;
		std::string			_key;
		unsigned int		_max_users;
	public:
		Channel();
		Channel(std::string name, int fd_creator);
		~Channel();

		bool				is_user(int	fd_user);
		
		void				add_user(int fd_user);
		void				kick_user(int fd_to_kick);
		void				part_user(int fd_user);

		void				set_topic(std::string topic);
		void				unset_topic();
		void				set_max_users(int max_users);


		std::string			get_name(void) const;
		std::vector<int>	get_users(void) const;
		std::string			get_topic(void) const;
		bool				get_topic_set(void) const;
		std::string			get_key(void) const;
		unsigned int		get_user_limit(void) const;

		void				broadcast(std::string message, int emitter);
};

#endif