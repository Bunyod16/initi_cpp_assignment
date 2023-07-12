NAME = string_sorter
SOURCES = test.cpp

OBJECTS = $(SOURCES:.cpp=.o)

CC = g++
CFLAGS = -Wall -Wextra -Werror -Wno-deprecated-declarations -std=c++11

all: $(NAME)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -D TEST_TYPE=full -g3 -o  $@ $^

simple: $(NAME)
	$(CC) $(CFLAGS) -D TEST_TYPE=simple -g3 -o  $@ $^

clean:
	$(RM) $(OBJECTS)
	
fclean: clean
	$(RM) $(NAME)

re: fclean all

lint:
	cpplint --filter=-legal/copyright $(SOURCES)

.PHONY: all clean fclean re lint