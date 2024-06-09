CC = cc
CFLAGS = -Wall -Wextra -Werror #-g -pthread -fsanitize=thread #-g -fsanitize=address
NAME = philo_bonus
SRC = main.c semaphores.c tools.c ft_atoi.c
OBJ = $(SRC:.c=.o)
HEADER = 

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
