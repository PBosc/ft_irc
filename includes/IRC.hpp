#ifndef IRC_HPP
# define IRC_HPP
# include <iostream>
# include <string>
# include <map>
# include <list>
# include <vector>
# include <utility>
# include <functional>
# include <stdexcept>
# include <algorithm>
# include <errno.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/epoll.h>
# include <fcntl.h>
# include <signal.h>
# include <sstream>
# include <ctime>
# include <cstdlib>
# include "Client.hpp"
# include "Channel.hpp"

# define MAX_CONNECTIONS 1000

extern t_data *g_data;

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
}				t_socket;

typedef struct s_command
{
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	parameters;
	std::string					last_param;
	bool						has_last_param;
}				t_command;

typedef struct 	s_data
{
        t_epoll								epoll;
        t_socket							socket;
        int									port;
        std::string							password;
        std::map<int, Client*>				clients;
        std::vector<int>					open_fds;
		std::vector<int>					operator_fds;
        std::map<std::string, Channel*>		channels;
}				t_data;

#endif