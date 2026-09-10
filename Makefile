CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
NAME = codexion

SRC = src/main.c src/parser.c src/utils.c \
      src/init.c src/validator.c src/simulation.c \
      src/simulation_utils.c src/dongles.c src/dongle_utils.c\
      src/monitor.c src/heap_queue.c src/heap_utils.c\
      src/cleanup.c 

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
