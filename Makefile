NAME = Durex

CC = gcc

FLAGS = -Wextra -Wall -Werror

SRC_DIR = src/

OBJS_DIR = objs/

INC = inc/

SRC =	main.c			\
		helpers.c		\
		createDurex.c	\
		daemon.c		\
		shell.c

OBJS = $(addprefix $(OBJS_DIR), $(SRC:.c=.o))

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $@ $(OBJS)

$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(FLAGS) -o $@ -c $< -I $(INC)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all