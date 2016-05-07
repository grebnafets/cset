SEARCH_PATH=-I$(CSET_SEARCH_PATH)

FLAGS=-std=gnu99 -pthread

CC=gcc $(SEARCH_PATH)
COMPILER=$(CC) $(FLAGS)

dirs=test

all: $(dirs)

$(dirs): %: ../cset/%
	cd $< && $(MAKE)
	cd $</api/cpp && $(MAKE)
	cd $</api/go && $(MAKE)
	cd $< && cp $@.h $(CSET_SEARCH_PATH)/build/so/$@.c
	$(CC) -c -fPIC so/$@.c -o so/$@.o
	$(COMPILER) so/$@.o -shared -o so/$@.so
	rm so/$@.o
	rm so/$@.c
