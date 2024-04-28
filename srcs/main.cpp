#include "IRC.hpp"

t_data *g_data;

int	find_user_fd(int fd, t_data &data)
{
	if (data.clients.find(fd) != data.clients.end())
		return fd;
	return -1;
}

bool	init(int &port, t_data &data)
{
	int	sock_opt_val = 1;

	data.socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (data.socket.fd < 0)
	{
		std::cerr << "Error: socket() failed" << std::endl;
		return false;
	}
	data.socket.addr = sockaddr_in();
	data.socket.addr.sin_family = AF_INET;
	data.socket.addr.sin_port = htons(port);
	data.socket.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (setsockopt(data.socket.fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt_val, sizeof(sock_opt_val)) < 0)
	{
		std::cerr << "Error: setsockopt() failed" << std::endl;
		return close(data.socket.fd), false;
	}
	if (bind(data.socket.fd, (struct sockaddr *)(&data.socket.addr), sizeof(data.socket.addr)) < 0)
	{
		std::cerr << "Error: bind() failed" << std::endl;
		return close(data.socket.fd), false;
	}
	if (listen(data.socket.fd, MAX_CONNECTIONS) < 0)
	{
		std::cerr << "Error: listen() failed" << std::endl;
		return close(data.socket.fd), false;
	}
	data.epoll.fd = epoll_create1(EPOLL_CLOEXEC);
	if (data.epoll.fd < 0)
	{
		std::cerr << "Error: epoll_create1() failed" << std::endl;
		return close(data.socket.fd), false;
	}
	data.socket.event.data.fd = data.socket.fd;
	data.socket.event.events = EPOLLIN;
	if (epoll_ctl(data.epoll.fd, EPOLL_CTL_ADD, data.socket.fd, &data.socket.event) < 0)
	{
		std::cerr << "Error: epoll_ctl() failed" << std::endl;
		return close(data.socket.fd), close(data.epoll.fd), false;
	}
	return true;
}

static void	user_connection(t_data &data)
{
	int					fd_new_con;
	struct	sockaddr_in socket_new_con;
	struct	epoll_event epoll_event_new_con;
	socklen_t			size_socket_new_con;
	Client				*new_client;
	static int			client_id = 1;

	size_socket_new_con = sizeof(socket_new_con);
	socket_new_con = sockaddr_in();
	epoll_event_new_con = epoll_event();
	fd_new_con = accept(data.socket.fd, (struct sockaddr *)&socket_new_con, &size_socket_new_con);
	if (fd_new_con < 0)
	{
		std::cerr << "Error: accept() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	new_client = new Client(client_id, fd_new_con);
	data.clients.insert(std::make_pair<int, Client *>(fd_new_con, new_client));
	data.open_fds.push_back(fd_new_con);
	epoll_event_new_con.events = EPOLLIN | EPOLLRDHUP;
	epoll_event_new_con.data.fd = fd_new_con;
	fcntl(fd_new_con, F_SETFL, O_NONBLOCK);
	if (epoll_ctl(data.epoll.fd, EPOLL_CTL_ADD, fd_new_con, &epoll_event_new_con) < 0)
	{
		std::cerr << "Error: epoll_ctl() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "User " << client_id << " connected" << std::endl;
	++client_id;
}

static void	user_disconnection(t_data &data, int fd)
{
	int							id_disc_user;
	Client						*disc_user;
	std::vector<int>::iterator	it;
	std::vector<int>::iterator	ite;

	epoll_ctl(data.epoll.fd, EPOLL_CTL_DEL, fd, &data.socket.event);
	close(fd);
	try
	{
		disc_user = data.clients.at(fd);
	}
	catch (out_of_range)
	{
		std::cerr << "Couldn't disconnect User with fd " << fd << ": no such user" << std::endl;
		return ;
	}
	id_disc_user = disc_user->get_id();
	data.clients.erase(fd);
	delete disc_user;
	it = data.open_fds.begin();
	ite = data.open_fds.end();
	for (; it != ite; it++)
	{
		if (*it == fd)
		{
			data.open_fds.erase(it);
			break;
		}
	}
	std::cout << "User " << id_disc_user << " disconnected" << std::endl;
}


void	run_serv(t_data &data, int i)
{
	const int user_fd = find_user_fd(data.epoll.events[i].data.fd, data);

	if (data.epoll.events[i].data.fd == data.socket.fd)
		user_connection(data);
	if ((data.epoll.events[i].events & EPOLLERR)
		|| (data.epoll.events[i].events & EPOLLHUP)
		|| (data.epoll.events[i].events & EPOLLRDHUP)
		|| !(data.epoll.events[i].events & EPOLLIN))
		user_disconnection(data, data.epoll.events[i].data.fd);
	//else if (user_fd != -1 && data.clients[user_fd]->get_id() != -1)
	//	user_command(user_fd, data);
} 

int main(int ac, char **av)
{
	t_data data;
	int epoll_fds;

	if (ac != 3) {
		std::cerr << "Usage: ./ft_irc <port> <password>" << std::endl;
		return 84;
	}
	data.port = std::stoi(av[1]);
	data.password = av[2];
	if (!init(data.port, data))
		return 84;
	g_data = &data;
	std::cout << "Port: " << data.port << std::endl << "Password: " << data.password << std::endl << "Server starting..." << std::endl;
	while (1)
	{
		epoll_fds = epoll_wait(data.epoll.fd, data.epoll.events, MAX_CONNECTIONS, -1);
		if (epoll_fds < 0)
		{
			std::cerr << "Error: epoll_wait failed" << std::endl;
			return (EXIT_FAILURE);
		}
		for (int i = 0; i < epoll_fds; i++)
			run_serv(data, i);		
	}
	return 0;
}