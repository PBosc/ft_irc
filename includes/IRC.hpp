#ifndef IRC_HPP
# define IRC_HPP
# include <algorithm>
# include <arpa/inet.h>
# include <cctype>
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

# define RESET "\033[0m"

# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# define MAX_CONNECTIONS 10
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
	std::string suffix;
	bool				has_suffix;
	std::vector<std::string> cmds;
}						t_command;

void					user_connection(void);
void					user_disconnection(int &i);
void					handle_message(int fd);
#endif
