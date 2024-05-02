# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/13 22:58:31 by wouhliss          #+#    #+#              #
#    Updated: 2024/05/02 16:00:44 by wouhliss         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
INCLUDE_PATH = ./includes
CPP_FLAGS = -Wall -Wextra -Werror -MMD -MP -std=c++98 -I $(INCLUDE_PATH)
RM = rm
RM_FLAGS = -rf

SRCS_PATH = ./srcs/
SRCS = 	${SRCS_PATH}main.cpp \
		${SRCS_PATH}connection.cpp \
		${SRCS_PATH}server.cpp \
		${SRCS_PATH}message.cpp

CLASS_PATH = ./classes/
CLASS = $(CLASS_PATH)Channel.cpp \
		$(CLASS_PATH)Client.cpp

OBJS = 	${SRCS:.cpp=.o} \
		${CLASS:.cpp=.o}

DEPS = ${SRCS:.cpp=.d}

NAME = ft_irc

all: ${NAME}

.cpp.o:
	${CPP} ${CPP_FLAGS} -c $< -o ${<:.cpp=.o}

${NAME}: ${OBJS}
	${CPP} ${CPP_FLAGS} ${OBJS} -o ${NAME}

clean:
	${RM} ${RM_FLAGS} ${OBJS} ${DEPS}

fclean: clean
	${RM} ${RM_FLAGS} ${NAME}

re: fclean
	make all

-include ${DEPS}

.PHONY: all clean fclean re