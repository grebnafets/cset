SEARCH_PATH=-I$(CSET_SEARCH_PATH)

FLAGS=-std=c++11 -Wall -Werror -Wextra -g -pthread

CC=g++ $(SEARCH_PATH)
COMPILER=$(CC) $(FLAGS)

all: main clean

main: main.o
	$(COMPILER) -o main main.o

main.o:
	$(CC) -c main.cpp

clean:
	rm -f *.o

run: main clean
	./main
