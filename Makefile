NAME = minishell

# CFLAGS = -Wall -Werror -Wextra
LIBFLAGS = -Llibft -lft -lreadline
INC = -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

SRCS = main.c \
		init.c \
		env_builtins.c \
		builtins.c \
		mem_utils.c \
		parse_utils.c \
		tokenizer.c \
		syntax.c \
		parser.c \
		heredocs.c \
		exec_utils.c \
		exec.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	cc -g $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(OBJS): $(SRCS)
	cc $(CFLAGS) -c $(SRCS) $(INC)

$(LIBFT):
	make -C $(LIBDIR)

clean:
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re