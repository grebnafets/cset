FLAGS=-std=c++11 -Wall -Werror -Wextra -g -pthread
SEARCH_PATH=-I$(CSET_SEARCH_PATH)

COMPILER=g++ $(SEARCH_PATH) $(FLAGS)

all: main clean

main: main.o
	$(COMPILER) -o main main.o

main.o:
	$(COMPILER) -c main.cpp

clean:
	rm -f *.o

run: main clean
	./main
