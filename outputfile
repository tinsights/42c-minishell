# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achak <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/31 14:40:12 by achak             #+#    #+#              #
#    Updated: 2024/03/31 16:51:46 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC	= cc -g -Wall -Werror -Wextra
SRCS	= $(wildcard *.c)

all:	$(NAME)

$(NAME):$(SRCS)
	$(CC) $(SRCS) -o $(NAME) -lreadline

test:
	make re
	clear && valgrind --leak-check=full ./$(NAME)

clean:
ifneq ("$(wildcard $(NAME))","")
	rm $(NAME)
endif

fclean:	clean

re:	fclean all

.PHONY:	all clean fclean re
