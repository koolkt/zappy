#
#   Makefile for zappy
#
#

SERVER =	zappy_server

SERVER_TESTS = server_tests

SERVER_DIR =	src/server

SERVER_TESTS_DIR = tests/server_tests/src/

CC =		gcc

CFLAGS =	-Wall -Werror -Wextra -g

RM =		rm -f

INC =		-I./include

###############"  SERVER ###############

SERVER_SOURCES = $(shell find $(SERVER_DIR)  -name "*.c")

SRVR_OBJ := $(SERVER_SOURCES:src/%.c=build/%.o)

DEPS =	$(shell find ./include  -name "*.h")

############### SERVER TESTS ###############

SERVER_TESTS_SOURCES = $(shell find $(SERVER_TESTS_DIR)  -name "*.c")

SRVR_TESTS_OBJ := $(SERVER_TESTS_SOURCES:tests/server_tests/src/%.c=tests/build/%.o)

############################################

build/%.o : src/%.c
	$(CC)  -c $< $(INC) -o $@

tests/build/%.o : tests/server_tests/src/%.c
	$(CC)  -c $< -I./tests/include $(INC) -o $@

all:		$(SERVER)  $(SERVER_TESTS)


$(SERVER_TESTS): $(SRVR_TESTS_OBJ) $(SRVR_OBJ) $(DEPS)
		$(CC) $(CFLAGS) $(INC) -lcunit -I./tests/include  $(SRVR_TESTS_OBJ) $(SRVR_OBJ:%main.o=) -o tests/bin/$(SERVER_TESTS)

$(SERVER):	$(SRVR_OBJ) $(DEPS)
		$(CC) $(CFLAGS) $(INC) $(SRVR_OBJ) -o bin/$(SERVER)
		cp bin/$(SERVER) .

runtests:
clean:
			$(RM) $(SRVR_OBJ) $(SRVR_TESTS_OBJ:%c=%.no)

fclean:		clean
			$(RM) $(SERVER) bin/$(SERVER) tests/bin/$(SERVER_TESTS)

re:			fclean $(SERVER) $(SERVER_TESTS)

.PHONY:			all clean fclean re
