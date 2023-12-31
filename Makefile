NAME = string_sorter
SOURCES = test.cpp

OBJECTS = $(SOURCES:.cpp=.o)

CC = g++
CFLAGS = -Wall -Wextra -Werror -Wno-deprecated-declarations -g3 -fsanitize=address -std=c++11

all: $(NAME)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

$(NAME): $(OBJECTS)
	$(CC)  $(CFLAGS) -o $@ $^

simple_objects:
	$(CC) -DSIMPLE_TEST -c $(CFLAGS) $(SOURCES)

simple: simple_objects
	$(CC) -DSIMPLE_TEST $(CFLAGS) -o simple_string_sorter $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME) simple_string_sorter

re: fclean all

lint:
	cpplint --filter=-legal/copyright $(SOURCES)

.PHONY: all clean fclean re lint