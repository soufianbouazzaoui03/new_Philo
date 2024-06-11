CC = cc
CFLAGS = -Wall -Wextra -Werror #-g  -fsanitize=address #-pthread -fsanitize=thread 
NAME = philo
SRC = main.c ft_atoi.c philo_tools.c monitor.c data.c
OBJ = $(SRC:.c=.o)
HEADER = philo.h

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

