FLAGS=-std=gnu99 -Wall -Werror -Wextra -g -pthread -finstrument-functions -fstack-protector-all
SEARCH_PATH=-I$(CSET_SEARCH_PATH)
LINKS=-lm

COMPILER=gcc $(SEARCH_PATH) $(FLAGS) $(LINKS)

all: main clean

main: main.o
	$(COMPILER) -o main main.o

main.o:
	$(COMPILER) -c main.c

clean:
	rm -f *.o

run: main clean
	./main
