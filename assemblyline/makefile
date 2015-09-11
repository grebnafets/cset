CFLAGS=-std=gnu99 -Wall -Werror -Wextra -g -pthread -finstrument-functions -fstack-protector-all
NOVAR=-Wno-unused-parameter -Wno-unused-variable -Wno-unused-function

CC=gcc $(CFLAGS)

all: main clean

main: main.o
	$(CC) -o main main.o

main.o:
	$(CC) -c main.c

clean:
	rm -f *.o

run: main clean
	./main



