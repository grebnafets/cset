SEARCH_PATH=-I$(CSET_SEARCH_PATH)

FLAGS=-std=gnu99 -Wall -Werror -Wextra -pthread
LINKS=-lm

CC=gcc -g $(SEARCH_PATH)
COMPILER=$(CC) $(FLAGS) $(LINKS)

all: main clean

main: main.o
	$(COMPILER) -o main main.o

main.o:
	$(CC) -c main.c

clean:
	rm -f *.o

run: main clean
	./main
