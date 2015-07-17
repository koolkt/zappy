
DIR_S_NET =			src/server/networking

DIR_S_GE =			src/server/game_engine

SOURCES = $(shell find $(DIR_S_NET)  -name "*.c")

SOURCES += $(shell find $(DIR_S_GE)  -name "*.c" )

SOURCES += src/server/main.c

OBJ := $(SOURCES:src/%.c=build/%.o)

build/%.o : src/%.c
	$(CC)  -c $< -Iinclude -o $@

DEPS =		

SERVER =	bin/zappy_server

CC =		gcc

CFLAGS =	-Wall -Werror -Wextra -g

RM =		rm -f

all:		$(SERVER)

$(SERVER):	$(OBJ)
		gcc -I./include  $(OBJ) -o $(SERVER)
		cp $(SERVER) .


clean:
			$(RM) build/$(OBJ)

fclean:		clean
			$(RM) $(SERVER) zappy_server

re:			fclean $(SERVER)

.PHONY:			all clean fclean re
