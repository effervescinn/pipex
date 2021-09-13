FILES = main.c cmds.c
OFILES = $(patsubst %.c,%.o,$(FILES))
DEPS = $(patsubst %.o,%.d,$(OFILES))
DEPFLAGS = -MMD -MF $(@:.o=.d)
CC = gcc
NAME = pipex
FLAGS = -Wall -Wextra -Werror
FT_LIB = $(LIBFT_DIR)/libft.a
LIBFT_DIR = ./libft

all:		$(NAME)

$(NAME):	$(OFILES)
			make -C $(LIBFT_DIR)
			$(CC) $(FLAGS) $(OFILES) $(FT_LIB) -o $(NAME)
-include $(DEPS)

%.o: 		%.c
			$(CC) $(FLAGS) -c $< $(DEPFLAGS) -o $(<:.c=.o)

clean:
			make clean -C $(LIBFT_DIR)
			rm -rf $(OFILES)
			rm -rf $(DEPS)

fclean:		clean
			make fclean -C $(LIBFT_DIR)
			rm -rf $(NAME) 

re:			fclean all


.PHONY:		all clean fclean re
