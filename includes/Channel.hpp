#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "IRC.hpp"

class Channel
{
  private:
	std::string _name;
	std::vector<int> _fds_users;
	std::string _topic;
	bool _is_topic_set;
	std::string _key;
	bool _is_key_set;
	unsigned int _max_users;
	bool _is_invite_only;
	bool _is_limit_set;
	unsigned int _limit;
	bool _is_topic_op_only;
	bool _is_operator_only;

  public:
	Channel();
	Channel(std::string name, int fd_creator);
	~Channel();

	bool is_user(int fd_user);

	void add_user(int fd_user);
	void kick_user(int fd_to_kick);
	void disconnect_user(int fd_to_disconnect);
	void part_user(int fd_user, const std::string &reason);

	void set_topic(std::string topic);
	void unset_topic();
	void set_max_users(int max_users);
	void set_invite_only(bool is_invite_only);
	void set_key(std::string key);
	void unset_key();
	void set_limit(unsigned int limit);
	void unset_limit();
	void set_topic_op_only(bool is_topic_op_only);
	void set_operator_only(bool is_operator_only);

	std::string get_name(void) const;
	std::vector<int> get_users(void) const;
	std::string get_topic(void) const;
	bool get_topic_set(void) const;
	std::string get_key(void) const;
	unsigned int get_user_limit(void) const;
	bool get_invite_only(void) const;
	bool get_key_set(void) const;
	bool get_limit_set(void) const;
	bool get_topic_op_only(void) const;
	bool get_operator_only(void) const;

	void broadcast(std::string &message, int emitter);
};

#endif