# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/20 13:03:28 by jmetayer          #+#    #+#              #
#    Updated: 2026/11/12 20:11:32 by jmetayer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJ_DIR = objects
OBJ_DIR_DEBUG = objects_debug

CC = c++ 
FLAGS = -Wall -Werror -Wextra -MMD -MP -std=c++98 -Iincludes
RM = rm -rf

SRCS = ./sources/main.cpp ./sources/IRC.cpp ./sources/Server/Server.cpp ./sources/Client.cpp ./sources/Message.cpp ./sources/Channel.cpp \
		./sources/Server/InitServerandPollLoop.cpp ./sources/Server/MessageTransmission.cpp ./sources/Server/SettersGetters.cpp \
		./sources/Server/Utils.cpp ./sources/Server/Commands/Display.cpp ./sources/Server/Commands/Pass.cpp ./sources/Server/Commands/Nick.cpp \
		./sources/Server/Commands/Join.cpp ./sources/Server/Commands/Kick.cpp ./sources/Server/Commands/Mode.cpp \
		./sources/Server/Commands/Privmsg.cpp ./sources/Server/Commands/User.cpp \
		./sources/Server/Commands/Invite.cpp ./sources/Server/Commands/Invite.cpp \

OBJ = $(patsubst ./sources/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJ:.o=.d)
DEPS_DEBUG = $(OBJ_DEBUG:.o=.d)
OBJ_DEBUG = $(patsubst ./sources/%.cpp, $(OBJ_DIR_DEBUG)/%.o, $(SRCS))

NAME = ircserv
NAME_DEBUG = ircserv_debug

all : $(NAME)

$(NAME)	: $(OBJ)
	$(CC) $(FLAGS) $^ -o $(NAME)
	@echo '$(NAME) compiled!'

$(OBJ_DIR)/%.o : ./sources/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

debug : $(NAME_DEBUG)

$(NAME_DEBUG) : $(OBJ_DEBUG)
	$(CC) $(FLAGS) $^ -o $(NAME_DEBUG)
	@echo '$(NAME_DEBUG) compiled!'

$(OBJ_DIR_DEBUG)/%.o : sources/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -D DEBUG_MODE=1 -c $< -o $@

clean :
	@$(RM) $(OBJ_DIR) $(OBJ_DIR) $(OBJ_DIR_DEBUG)

fclean : clean
	@$(RM) $(NAME) $(NAME_DEBUG)

re : fclean all

-include $(DEPS) $(DEPS_DEBUG)

.PHONY : all clean fclean re
