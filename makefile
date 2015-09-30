CFLAGS=-std=gnu99 -Wall -Werror -Wextra -g -pthread -finstrument-functions -fstack-protector-all
CPPFLAGS=-std=c++11 -Wall -Werror -Wextra -g -pthread -finstrument-functions -fstack-protector-all
NOVAR=-Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
SEARCHP=-I$(CSET_SEARCH_PATH)

GCC=gcc $(SEARCHP) $(CFLAGS)
GPP=g++ $(SEARCHP) $(CPPFLAGS)

all: main mainpp clean

mainpp: mainpp.o
	$(GPP) -o mainpp mainpp.o

main: main.o
	$(GCC) -o main main.o

mainpp.o:
	$(GPP) -c mainpp.cpp

main.o:
	$(GCC) -c main.c

clean:
	rm -f *.o

run: main clean
	./main



