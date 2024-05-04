/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:40:32 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/04 04:17:12 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# include "IRC.hpp"

typedef int (Client::*ScriptFunction)(t_command &);

class Server
{

  public:
	Server(void);
	~Server(void);
	t_epoll &get_epoll(void);
	void set_epoll(t_epoll &epoll);
	t_socket &get_socket(void);
	void set_socket(t_socket &socket);
	int &get_port(void);
	void set_port(int &port);
	std::string &get_password(void);
	void set_password(std::string &string);
	std::map<std::string, Channel *> &get_channels(void);
	void set_channel(std::string &channel_name, Channel *channel);
	std::map<int, Client *> &get_clients(void);
	void set_client(int &fd, Client *client);
	int &get_clients_id(void);
	Client *find_client_by_nick(std::string &nick);
	std::map<std::string, ScriptFunction> &get_commands(void);
	std::map<std::string, std::string> &get_operators(void);
	void set_clients_id(int &clients_id);
	bool init(int port, std::string password);
	void run(int &i);
	void kick_user(int fd);
  private:
	t_epoll _epoll;
	t_socket _socket;
	int _port;
	std::string _password;
	std::map<std::string, std::string>_operators;
	std::map<std::string, Channel *> _channels;
	std::map<int, Client *> _clients;
	std::map<std::string, ScriptFunction> _commands;
	int _clients_id;


};
std::ostream &operator<<(std::ostream &os, Server &server);
extern Server	g_server;
#endif
