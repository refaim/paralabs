.PHONY: clean

INCLUDES=-I. -I/usr/include/nptl
CFLAGS=-std=c99 -D_REENTERANT $(INCLUDES)
LDFLAGS=-L/usr/lib/nptl -lpthread
clean:
	rm -f t01 t02 t03

t%: t@.c

