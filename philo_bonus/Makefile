NAME = philo_bonus
CFLAGS = -Wall -Werror -Wextra
SRC = mutex_thread.c utils.c main.c

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

.PHONY: all clean fclean re

%.o: %.c
	gcc $(CFLAGS) -c $< -o $(<:.c=.o)	

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ)
	
all: $(NAME)

clean: 
	rm -rf $(OBJ) $(OBJ_BONUS)

fclean:	clean
	rm -rf $(NAME)

re: fclean $(NAME)