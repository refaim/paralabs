.PHONY: all clean

CFLAGS += -Wall -Wextra -pedantic -std=c99 -pthread

SOURCES = $(wildcard task*.c)
BINARIES = $(patsubst %.c, %, $(SOURCES))

all: $(BINARIES)

clean:
	$(RM) $(BINARIES)

define define_task
$(1): $(1).c
	$(CC) -o $$@ $(CFLAGS) $$^
endef

define pthread_task
	$(eval $(call define_task, task$$$(1)))
endef

$(foreach number, 01 02 03 04, $(call pthread_task, $(number)))
