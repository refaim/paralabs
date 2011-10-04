.PHONY: clean

CFLAGS += -Wall -Wextra -pedantic -std=c99

BINARIES = $(patsubst %.c, %, $(wildcard *.c))

all: $(BINARIES)

%_pt: %_pt.c
	$(CC) -o $@ $(CFLAGS) -pthread $(LDFLAGS) $^

%_omp: %_omp.c
	$(CC) -o $@ $(CFLAGS) -fopenmp $(LDFLAGS) $^

clean:
	$(RM) $(BINARIES)
