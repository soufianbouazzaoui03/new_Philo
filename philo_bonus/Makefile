CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = philo_bonus
SRC = main.c semaphores.c tools.c ft_atoi.c data.c
OBJ = $(SRC:.c=.o)
HEADER = philo_bonus.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re : fclean all
