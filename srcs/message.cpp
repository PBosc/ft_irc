/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:54:39 by wouhliss          #+#    #+#             */
/*   Updated: 2024/05/02 16:47:22 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"

// void	handle_message(t_data &data, int fd)
// {
// 	char		buf[BUFFER_SIZE + 1];
// 	ssize_t		rc;
// 	bool		closed;
	
// 	(void)data;
// 	std::cout << "User " << fd << " is sending a command" << std::endl;
// 	closed = false;
// 	while (true)
// 	{
// 		rc = recv(fd, buf, BUFFER_SIZE, 0);
// 		if (rc < 0)
// 		{
// 			if (errno != EWOULDBLOCK)
// 			{
// 				perror("recv()");
// 				closed = true;
// 			}
// 			std::cout << "User " << fd << " sent: " << message << std::endl;
// 			break ;
// 		}
// 		buf[rc] = 0;
// 		message.append(buf);
// 	}
// }
