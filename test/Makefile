OUTNAME = map_test

GCC = gcc
GCCFLAGS:= -O0 -ggdb -pg
INCLUDE:= -I./deps/

all: map_test

deps:
	clib install jlcordeiro/minunit

map_test: clean deps
	$(GCC) $(GCCFLAGS) $(INCLUDE) -c map_test.c
	$(GCC) -o $(OUTNAME) map_test.o ../map.o

clean:
	@rm -f *.o $(OUTNAME)

distclean: clean
	-@rm -rf ./deps

format:
	astyle --options=$(HOME)/.astyle_format.conf *.c *.h
