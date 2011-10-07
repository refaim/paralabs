.PHONY: all clean

CFLAGS += -Wall -Wextra -pedantic -std=c99
CHECK_FLAGS = -Wl,--whole-archive -lcheck -Wl,--no-whole-archive

PREFIX = task
BINARIES = $(patsubst %.c, %, $(wildcard $(PREFIX)*.c))

all: $(BINARIES)

clean:
	$(RM) $(BINARIES)

define define_task
$(1): $(1).c $(3)
	$(CC) -o $$@ $(CFLAGS) $(2) $(LDFLAGS) $$^
endef

define pthread_task
	$(eval $(call define_task, $(PREFIX)$$$(1), -pthread, $(2)))
endef

define openmp_task
	$(eval $(call define_task, $(PREFIX)$$$(1), -fopenmp, $(2)))
endef

$(foreach number, 01 02 03, $(call pthread_task, $(number)))
$(call openmp_task, 04)
LDFLAGS += $(CHECK_FLAGS)
$(call openmp_task, 05, sort.c quicksort.c)
