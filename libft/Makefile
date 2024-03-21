NAME = libft.a
DIR = $(shell pwd)
SRC_DIR = $(DIR)/sources
SOURCES = $(wildcard $(SRC_DIR)/*.c)

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(SRC_DIR)/%.o,$(SOURCES))

CFLAGS = -Wall -Wextra -Werror -I./includes

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $(NAME) $(OBJECTS)

$(OBJECTS): $(SRC_DIR)/%.o : $(SRC_DIR)/%.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
