.PHONY: clean

INCLUDES = -I$(CURDIR)
CFLAGS += -Wall -Wextra -pedantic -std=c99

BINARIES = $(patsubst %.c, %, $(wildcard *.c))

all: $(BINARIES)

%_pt: %_pt.c
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -pthread $(LDFLAGS) $^

%_omp: %_omp.c
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -fopenmp $(LDFLAGS) $^

clean:
	$(RM) $(BINARIES)
