#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"

t_data	*g_data;
bool	end = false;

void	handle_quit(int sig)
{
	(void)sig;
	end = true;
}

bool	init(int &port, t_data &data)
{
	int	sock_opt_val;

	sock_opt_val = 1;
	data.socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (data.socket.fd < 0)
	{
		std::cerr << "Error: socket() failed" << std::endl;
		return (false);
	}
	data.clients = std::map<int, Client *>();
	data.socket.addr = sockaddr_in();
	data.socket.addr.sin_family = AF_INET;
	data.socket.addr.sin_port = htons(port);
	data.socket.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (setsockopt(data.socket.fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt_val,
			sizeof(sock_opt_val)) < 0)
	{
		std::cerr << "Error: setsockopt() failed" << std::endl;
		return (close(data.socket.fd), false);
	}
	if (bind(data.socket.fd, (struct sockaddr *)(&data.socket.addr),
			sizeof(data.socket.addr)) < 0)
	{
		std::cerr << "Error: bind() failed" << std::endl;
		return (close(data.socket.fd), false);
	}
	if (listen(data.socket.fd, MAX_CONNECTIONS) < 0)
	{
		std::cerr << "Error: listen() failed" << std::endl;
		return (close(data.socket.fd), false);
	}
	data.epoll.fd = epoll_create1(EPOLL_CLOEXEC);
	if (data.epoll.fd < 0)
	{
		std::cerr << "Error: epoll_create1() failed" << std::endl;
		return (close(data.socket.fd), false);
	}
	data.socket.event.data.fd = data.socket.fd;
	data.socket.event.events = EPOLLIN;
	if (epoll_ctl(data.epoll.fd, EPOLL_CTL_ADD, data.socket.fd,
			&data.socket.event) < 0)
	{
		std::cerr << "Error: epoll_ctl() failed" << std::endl;
		return (close(data.socket.fd), close(data.epoll.fd), false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		epoll_fds;

	if (ac != 3)
	{
		std::cerr << "Usage: ./ft_irc <port> <password>" << std::endl;
		return (84);
	}
	data.port = std::atoi(av[1]);
	data.password = av[2];
	g_data = &data;
	if (!init(data.port, data))
		return (84);
	std::cout << "Port: " << data.port << std::endl << "Password: " << data.password << std::endl << "Server starting..." << std::endl;
	signal(SIGINT, handle_quit);
	while (!end)
	{
		epoll_fds = epoll_wait(data.epoll.fd, data.epoll.events,
				MAX_CONNECTIONS, 3000);
		if (epoll_fds < 0 && !end)
		{
			std::cerr << "Error: epoll_wait failed" << std::endl;
			return (EXIT_FAILURE);
		}
		for (int i = 0; i < epoll_fds; i++)
			run_serv(data, i);
	}
	return (0);
}
