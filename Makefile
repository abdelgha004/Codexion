CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -Iinclude

RM = rm -f

NAME = codexion

SRC = src/main.c \
	src/parser.c \
	src/init.c \
	src/cleanup.c \
	src/simulation.c \
	src/monitor.c \
	src/coder.c \
	src/dongle.c \
	src/heap.c \
	src/time.c \
	src/utils.c

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