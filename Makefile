# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/13 22:58:31 by wouhliss          #+#    #+#              #
#    Updated: 2024/05/04 04:49:16 by ybelatar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
INCLUDE_PATH = ./includes
CPP_FLAGS = -Wall -Wextra -Werror -MMD -MP -std=c++98 -O3 -I $(INCLUDE_PATH)
RM = rm
RM_FLAGS = -rf

SRCS_PATH = ./srcs/
SRCS = 	${SRCS_PATH}main.cpp \
		${SRCS_PATH}connection.cpp \
		${SRCS_PATH}message.cpp

CLASS_PATH = ./classes/
COMMAND_PATH = ./classes/commands/
CLASS = $(CLASS_PATH)Channel.cpp \
		$(CLASS_PATH)Client.cpp \
		$(CLASS_PATH)Server.cpp \
		$(COMMAND_PATH)CAP.cpp \
		$(COMMAND_PATH)INVITE.cpp \
		$(COMMAND_PATH)JOIN.cpp \
		$(COMMAND_PATH)KICK.cpp \
		$(COMMAND_PATH)KILL.cpp \
		$(COMMAND_PATH)MODE.cpp \
		$(COMMAND_PATH)NAMES.cpp \
		$(COMMAND_PATH)NICK.cpp \
		$(COMMAND_PATH)NOTICE.cpp \
		$(COMMAND_PATH)OPER.cpp \
		$(COMMAND_PATH)PART.cpp \
		$(COMMAND_PATH)PASS.cpp \
		$(COMMAND_PATH)PING.cpp \
		$(COMMAND_PATH)PRIVMSG.cpp \
		$(COMMAND_PATH)QUIT.cpp \
		$(COMMAND_PATH)TOPIC.cpp \
		$(COMMAND_PATH)USER.cpp \
		$(COMMAND_PATH)UNKNOWN.cpp

OBJS = 	${SRCS:.cpp=.o} \
		${CLASS:.cpp=.o}

OBJSDIR = objs/

OBJS := $(addprefix $(OBJSDIR), $(OBJS))

DEPS = 	${SRCS:.cpp=.d} \
		${CLASS:.cpp=.d}

NAME = ircserv

all: ${NAME}

# .cpp.o:
# 	${CPP} ${CPP_FLAGS} -c $< -o ${<:.cpp=.o}
	
$(OBJSDIR)%.o: %.cpp
	@mkdir -p $(@D)
	${CPP} ${CPP_FLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CPP} ${CPP_FLAGS} ${OBJS} -o ${NAME}

clean:
	${RM} ${RM_FLAGS} ${OBJSDIR}

fclean: clean
	${RM} ${RM_FLAGS} ${NAME}

re: fclean
	make all

-include ${DEPS}

.PHONY: all clean fclean re