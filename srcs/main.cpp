#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"

Server	g_server;
bool	end = false;

void	handle_quit(int sig)
{
	(void)sig;
	end = true;
}

void __attribute__((destructor)) on_exit(void)
{
	if (g_server.get_socket().fd > 0)
		close(g_server.get_socket().fd);
	if (g_server.get_epoll().fd > 0)
		close(g_server.get_epoll().fd);
}

int	main(int ac, char **av)
{
	int				epoll_fds;

	if (ac != 3)
	{
		std::cerr << "Usage: ./ft_irc <port> <password>" << std::endl;
		return (84);
	}
	if (!g_server.init(std::atoi(av[1]), std::string(av[2])))
		return (84);
	std::cout << g_server << std::endl;
	signal(SIGINT, handle_quit);
	while (!end)
	{
		epoll_fds = epoll_wait(g_server.get_epoll().fd, g_server.get_epoll().events,
				MAX_CONNECTIONS, 3000);
		if (epoll_fds < 0 && !end)
		{
			std::cerr << "Error: epoll_wait failed" << std::endl;
			return (EXIT_FAILURE);
		}
		for (int i = 0; i < epoll_fds; i++)
			g_server.run(i);
	}
	return (0);
}
