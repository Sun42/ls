NAME = ls
SRC = 	ls.c \
	args.c \
	utils_list.c \
	display.c \
	generic.c \
	long_format.c \
	dir_to_list.c
	
OBJ = $(SRC:.c=.o)
CFLAGS += -I/u/all/sundas_c/rendu/include -I. -W -Wall -ansi -pedantic
INCLUDES = -L/u/all/sundas_c/rendu/lib/my -lmy
RM = rm -f
CC = gcc

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(INCLUDES)
	

all: 	$(NAME)
	cp $(NAME) test/$(NAME)
clean :
	$(RM) $(OBJ)
	$(RM) *~

fclean: clean
	$(RM) $(NAME)

.PHONY: all clean fclean re
