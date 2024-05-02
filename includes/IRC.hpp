#ifndef IRC_HPP
# define IRC_HPP
# include <algorithm>
# include <arpa/inet.h>
# include <cstdlib>
# include <ctime>
# include <errno.h>
# include <fcntl.h>
# include <functional>
# include <iostream>
# include <list>
# include <map>
# include <netinet/in.h>
# include <signal.h>
# include <sstream>
# include <stdexcept>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <string>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <utility>
# include <vector>

# define MAX_CONNECTIONS 1000
# define BUFFER_SIZE 4096

class					Client;
class					Channel;

typedef struct s_epoll
{
	int					fd;
	epoll_event			events[MAX_CONNECTIONS];
}						t_epoll;

typedef struct s_socket
{
	int					fd;
	epoll_event			event;
	struct sockaddr_in	addr;
}						t_socket;

typedef struct s_command
{
	std::string prefix;
	std::string command;
	std::vector<std::string> parameters;
	std::string last_param;
	bool				has_last_param;
}						t_command;

typedef struct s_data
{
	t_epoll				epoll;
	t_socket			socket;
	int					port;
	std::string password;
	std::map<std::string, Channel *> channels;
	std::map<int, Client *> clients;
	int					client_id;
}						t_data;

extern t_data			*g_data;

void					run_serv(t_data &data, int i);
void					user_connection(t_data &data);
void					user_disconnection(t_data &data, int i);
void					handle_message(t_data &data, int fd);
#endif