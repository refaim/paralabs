.PHONY: clean

CFLAGS += -D_REENTERANT -I/usr/include/nptl -I. -L/usr/lib/nptl -lpthread -std=c99

clean:
	rm -f t01 t02 t03

t%: t@.c

