# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmetayer <jmetayer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/20 13:03:28 by jmetayer          #+#    #+#              #
#    Updated: 2026/10/20 16:55:41 by jmetayer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJ_DIR = objects

CC = c++ 
FLAGS = -Wall -Werror -Wextra -std=c++98 -Iincludes
RM = rm -rf

SRCS = ./sources/main.cpp ./sources/IRC.cpp
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

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

.PHONY : all clean fclean re
