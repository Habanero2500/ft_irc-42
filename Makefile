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


NAME = ircserv
SRCS = ./sources/main.cpp ./sources/IRC.cpp

CC = c++ 
FLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -f
OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

all : $(NAME)

$(NAME)	: $(OBJS)
	@$(CC) $(FLAGS) $^ -o $(NAME)
	@echo '$(NAME) compiled!'

fclean : clean
	@$(RM) $(NAME)

clean :
	@$(RM) $(OBJS)

re			: fclean all

.PHONY : all clean fclean re