NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g
LIBFLAGS = -Llibft -lft -lreadline
INC = -Ilibft/includes -Iincs

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	# might be version dependent, unsure
	# also have to have readline installed
	INC += -I/opt/homebrew/opt/readline/include
	LIBFLAGS += -L/opt/homebrew/opt/readline/lib -L/usr/local/lib
endif

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

SRCDIR = srcs/
OBJDIR = objs/
INCDIR = incs/

HDRS = $(addprefix $(INCDIR), minishell.h)
SRCS =  main.c \
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

OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.c $(HDRS)
	cc $(CFLAGS) -c $< $(INC) -o $@

$(LIBFT):
	make -C $(LIBDIR)

clean:
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re