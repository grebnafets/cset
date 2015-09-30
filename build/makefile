dirs=test ts support badcontext mem f logger critical safefork assemblyline  \
     threadwrapp hashmap charmap

all: $(dirs)

$(dirs): %: ../cset/%
	cd $< && $(MAKE)
	cd $</cpp && $(MAKE)
