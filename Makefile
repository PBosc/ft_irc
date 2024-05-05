# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/13 22:58:31 by wouhliss          #+#    #+#              #
#    Updated: 2024/05/05 18:05:27 by ybelatar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
INCLUDE_PATH = includes
CPP_FLAGS = -Wall -Wextra -Werror -MMD -MP -std=c++98 -g3 -I $(INCLUDE_PATH)
RM = rm
RM_FLAGS = -rf

SRCS_PATH = srcs/
SRCS = 	${SRCS_PATH}main.cpp \
		${SRCS_PATH}connection.cpp \
		${SRCS_PATH}message.cpp

CLASS_PATH = classes/
COMMAND_PATH = classes/commands/
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
		$(COMMAND_PATH)WHO.cpp \
		$(COMMAND_PATH)UNKNOWN.cpp

BOT_SRCS = srcs/bot/main.cpp

OBJS = 	${SRCS:.cpp=.o} \
		${CLASS:.cpp=.o}

BOT_OBJS = ${BOT_SRCS:.cpp=.o}

BOT_DEPS =	${BOT_SRCS:.cpp=.d}

OBJSDIR = objs/

OBJS := $(addprefix $(OBJSDIR), $(OBJS))

BOT_OBJS := $(addprefix $(OBJSDIR), $(BOT_OBJS))

DEPS = 	${SRCS:.cpp=.d} \
		${CLASS:.cpp=.d}
	
DEPS := $(addprefix $(OBJSDIR), $(DEPS))

BOT_DEPS := $(addprefix $(OBJSDIR), $(BOT_DEPS))

NAME = ircserv
BOT_NAME = bot

all: ${NAME} ${BOT_NAME}

# .cpp.o:
# 	${CPP} ${CPP_FLAGS} -c $< -o ${<:.cpp=.o}
	
$(OBJSDIR)%.o: %.cpp
	@mkdir -p $(@D)
	${CPP} ${CPP_FLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CPP} ${CPP_FLAGS} ${OBJS} -o ${NAME}

${BOT_NAME} : ${BOT_OBJS} 
	${CPP} ${CPP_FLAGS} ${BOT_OBJS} -o ${BOT_NAME}

clean:
	${RM} ${RM_FLAGS} ${OBJSDIR}

fclean: clean
	${RM} ${RM_FLAGS} ${NAME}

re: fclean
	make all

-include ${DEPS} ${BOT_DEPS}

.PHONY: all clean fclean re