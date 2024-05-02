/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:40:32 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/02 22:14:20 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# include "IRC.hpp"

class Server
{
  public:
	~Server(void);
	t_epoll &get_epoll(void) const;
	void set_epoll(t_epoll &epoll);
	t_socket &get_socket(void) const;
	void set_socket(t_socket &socket);
	int &get_port(void) const;
	void set_port(int &port);
	std::string &get_password(void) const;
	void set_password(std::string &string);
	std::map<std::string, Channel *> &get_channels(void) const;
	void set_channel(std::string &channel_name, Channel *channel);
	std::map<int, Client *> &get_clients(void) const;
	void set_client(int &fd, Client *client);
	int &get_clients_id(void) const;
	void set_clients_id(int &clients_id);
	bool init(int &port, std::string &password);
	void run(int &i);

  private:
	t_epoll _epoll;
	t_socket _socket;
	int _port;
	std::string _password;
	std::map<std::string, Channel *> _channels;
	std::map<int, Client *> _clients;
	int _clients_id;
};
#endif