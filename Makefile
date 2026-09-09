CC = cc
# CFLAGS = -Wall -Wextra -Werror -pthread

CFLAGS = -Wall -Wextra -Werror -Wno-error=type-limits -pthread
NAME = codexion

SRC = SRC/utils.c SRC/parser.c SRC/validator.c SRC/initialize_data.c \
    SRC/main.c SRC/cleanup.c SRC/simulation.c SRC/simulation_utils.c \
    SRC/dongles.c SRC/dongle_utils.c SRC/monitor.c SRC/heap_queue.c \
    SRC/heap_utils.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
