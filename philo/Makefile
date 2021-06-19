NAME = philo
CFLAGS = -Wall -Werror -Wextra
SRC =

SRC_BONUS =	

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

.PHONY: all clean fclean re

%.o: %.c
	gcc $(CFLAGS) -c $< -o $(<:.c=.o)	

$(NAME): $(OBJ)
	
all: $(NAME)
	gcc -o $(NAME) 

clean: 
	rm -rf $(OBJ) $(OBJ_BONUS)

fclean:	clean
	rm -rf $(NAME)

bonus: $(NAME) $(OBJ) $(OBJ_BONUS)
	ar -rcs $(NAME) $(OBJ) $(OBJ_BONUS)

re: fclean $(NAME)