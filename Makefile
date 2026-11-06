# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: opapouts <opapouts@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/20 13:03:28 by jmetayer          #+#    #+#              #
#    Updated: 2026/11/06 16:08:57 by opapouts         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJ_DIR = objects
OBJ_DIR_DEBUG = objects_debug

CC = c++ 
FLAGS = -Wall -Werror -Wextra -MMD -MP -std=c++98 -Iincludes
RM = rm -rf

SRCS = ./sources/main.cpp ./sources/IRC.cpp ./sources/Server.cpp ./sources/Client.cpp ./sources/Message.cpp
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS = $(OBJ:.o=.d)
DEPS_DEBUG = $(OBJ_DEBUG:.o=.d)
OBJ_DEBUG = $(addprefix $(OBJ_DIR_DEBUG)/, $(notdir $(SRCS:.cpp=.o)))

NAME = ircserv
NAME_DEBUG = ircserv_debug

all : $(NAME)

$(NAME)	: $(OBJ)
	$(CC) $(FLAGS) $^ -o $(NAME)
	@echo '$(NAME) compiled!'

$(OBJ_DIR)/%.o : sources/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

debug : $(NAME_DEBUG)

$(NAME_DEBUG) : $(OBJ_DEBUG)
	$(CC) $(FLAGS) $^ -o $(NAME_DEBUG)
	@echo '$(NAME_DEBUG) compiled!'

$(OBJ_DIR_DEBUG)/%.o : sources/%.cpp
	@mkdir -p $(OBJ_DIR_DEBUG)
	$(CC) $(FLAGS) -D DEBUG_MODE=1 -c $< -o $@

clean :
	@$(RM) $(OBJ_DIR) $(OBJ_DIR) $(OBJ_DIR_DEBUG)

fclean : clean
	@$(RM) $(NAME) $(NAME_DEBUG)

re : fclean all

-include $(DEPS) $(DEPS_DEBUG)

.PHONY : all clean fclean re
