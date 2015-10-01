
FLAGS=-std=gnu99 -pthread
SEARCH_PATH=-I$(CSET_SEARCH_PATH)

COMPILER=gcc $(SEARCH_PATH) $(FLAGS)

dirs=test ts support badcontext mem f logger critical safefork assemblyline  \
     threadwrapp hashmap charmap

all: $(dirs)

$(dirs): %: ../cset/%
	cd $< && $(MAKE)
	cd $</api/cpp && $(MAKE)
	cd $< && cp $@.h $(CSET_SEARCH_PATH)/build/so/$@.c
	$(COMPILER) -c -fPIC so/$@.c -o so/$@.o
	$(COMPILER) so/$@.o -shared -o so/$@.so
	rm so/$@.o
	rm so/$@.c
