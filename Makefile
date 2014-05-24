GCC := gcc
GCCFLAGS:= -Werror -Wall -Wextra -pedantic -D_POSIX_SOURCE -D_GNU_SOURCE -std=c99 \
	-Wno-missing-braces -Wextra -Wformat=2 \
	-Wswitch-default -Wswitch -Wcast-align -Wpointer-arith \
	-Wundef -Wcast-qual -Wshadow -Winline \
	-Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
	-fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing \
	-Wconversion -Wno-missing-field-initializers

GCCEXTRAFLAGS := $(shell expr `$(GCC) -dumpversion` \>= 4.6)
ifeq "$(GCCEXTRAFLAGS)" "1"
	GCCFLAGS += -Wlogical-op -Wstrict-overflow=5 -Wunreachable-code
endif

GCCFLAGS+= -O0 -ggdb -pg

all: cmap

cmap:
	$(GCC) $(GCCFLAGS) -c map.c

tests: cmap
	make -C test

clean:
	-@rm map.o

format:
	astyle --options=$(HOME)/.astyle_format.conf *.c *.h
