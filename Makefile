CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

NAME = codexion

SRC = src/main.c src/parsing.c src/coder.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re