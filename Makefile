CC = cc
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror

SRC = philo.c \
	  ft_atoi.c init.c mutex.c sim.c threads.c utils.c 

OBJ = $(SRC:.c=.o)

NAME = philo

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
