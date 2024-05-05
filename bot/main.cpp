#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include "IRC.hpp"

bool g_end = false;
int g_fd = -1;
std::vector<std::string> g_ban_words;
std::map<std::string, std::string> g_bot_responses;

void handle_quit(int sig)
{
    (void)sig;
    if (g_fd > 0)
        close(g_fd);
    g_end = true;
}
t_command parse_command(std::string line)
{
	t_command cmd;
	
	std::stringstream ss(line);
	std::string word;

	cmd.has_suffix = false;
	ss >> word;
	unsigned long pos = word.find(":");
	while (pos == 0)
	{
		cmd.prefix.append(word);
		ss >> word;
		pos = word.find(":");
	}
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	cmd.command = word;
	while (ss >> word && word[0] != ':')
	{
		cmd.parameters.push_back(word);
	}
	bool first = true;
	while (ss)
	{
		cmd.has_suffix = true;
		if (!first)
			cmd.suffix.append(" ");
		if (first && word[0] == ':')
			word.erase(0, 1);
		first = false;
		cmd.suffix.append(word);
		ss >> word;
	}
	return (cmd);
}

int main(int ac, char **av)
{
    char    buf[4096 + 1];
    int ret;
    if (ac != 5)
    {
        std::cerr << "Usage: ./bot <nick> <host> <port> <pass>" << std::endl;
        return (84);
    }
    std::string nick(av[1]);
    std::string host(av[2]);
    std::string port(av[3]);
    std::string pass(av[4]);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(std::atoi(port.c_str()));
    if (addr.sin_port == 0)
    {
        std::cerr << "Error: htons() failed" << std::endl;
        return (84);
    }
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        std::cerr << "Error: inet_addr() failed" << std::endl;
        return (84);
    }
    g_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_fd < 0)
    {
        std::cerr << "Error: socket() failed" << std::endl;
        return (84);
    }
    if (connect(g_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Error: connect() failed" << std::endl;
        return (84);
    }
    if (send(g_fd, "CAP BOT\n", 8, 0) < 0)
    {
        std::cerr << "Error: send() failed" << std::endl;
        return (84);
    }
    signal(SIGINT, handle_quit);
    while (!g_end)
    {
        ret = read(g_fd, buf, 4096);
        if (ret <= 0)
        {
            if (ret < 0)
                std::cerr << "Error: read() failed" << std::endl;
            return (84);
        }
        buf[ret] = 0;
        std::cout << buf << std::endl;
        std::string message(buf);
        t_command cmd = parse_command(message);
        std::cout << "Command: " << cmd.command << std::endl;
        std::cout << "Prefix: " << cmd.prefix << std::endl;
        if (cmd.command == "PRIVMSG")
        {
            if (cmd.suffix.find("!add_ban_word") == 0)
            {
                std::cout << "Adding ban word" << std::endl;
                std::string word = cmd.suffix.substr(14);
                if (word.empty())
                {
                    send(g_fd, ("PRIVMSG " + cmd.prefix + " :Invalid command\n").c_str(), 50, 0);
                    continue;
                }
                std::cout << "Adding ban word: " << word << std::endl;
                g_ban_words.push_back(word);
            }
            else
            {
                std::cout << "Checking for ban words" << std::endl;
                for (std::vector<std::string>::iterator it = g_ban_words.begin(); it != g_ban_words.end(); it++)
                {
                    if (message.find(*it) != std::string::npos)
                    {
                        std::cout << "Swearing detected" << std::endl;
                        std::string to_ban = cmd.prefix.substr(1, cmd.prefix.find("!") - 1);
                        send(g_fd, ("KILL " + to_ban + " :Swearing detected\n").c_str(), 50, 0);
                        break;
                    }
                }
            }
        }
    }
    return (0);
}
