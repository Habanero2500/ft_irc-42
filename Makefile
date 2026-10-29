# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malaimo <malaimo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/20 13:03:28 by jmetayer          #+#    #+#              #
#    Updated: 2026/10/29 17:12:44 by malaimo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJ_DIR = objects

CC = c++ 
FLAGS = -Wall -Werror -Wextra -MMD -MP -std=c++98 -Iincludes
RM = rm -rf

SRCS = ./sources/main.cpp ./sources/IRC.cpp ./sources/Server.cpp ./sources/Client.cpp
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.d))

NAME = ircserv

all : $(NAME)

$(NAME)	: $(OBJ)
	$(CC) $(FLAGS) $^ -o $(NAME)
	@echo '$(NAME) compiled!'

$(OBJ_DIR)/%.o : sources/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean :
	@$(RM) $(OBJ_DIR)

fclean : clean
	@$(RM) $(NAME)

re : fclean all

debug : FLAGS += -D DEBUG_MODE=1
debug : re

.PHONY : all clean fclean re
