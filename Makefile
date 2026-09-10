CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
NAME = codexion

SRC = src/main.c src/parser.c src/validator.c \
      src/init.c src/cleanup.c src/coder.c \
      src/coder_utils.c src/dongle.c src/dongle_utils.c \
      src/scheduler.c src/scheduler_utils.c src/monitor.c \
      src/time.c src/helpers.c

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
