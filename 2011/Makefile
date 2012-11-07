.PHONY: all clean

CFLAGS += -Wall -Wextra -pedantic -std=c99
CHECK_LDFLAGS = -Wl,--whole-archive -lcheck -Wl,--no-whole-archive

PREFIX = task
BINARIES = $(patsubst %.c, %, $(wildcard $(PREFIX)*.c))

all: $(BINARIES)

clean:
	$(RM) $(BINARIES)

define define_task
$(1): $(1).c common.c $(3)
	$(CC) -o $$@ $(CFLAGS)$(2) $(LDFLAGS) $$^
endef

define pthread_task
	$(eval $(call define_task, $(PREFIX)$$$(1), -pthread, $(2)))
endef

define openmp_task
	$(eval $(call define_task, $(PREFIX)$$$(1), -fopenmp, $(2)))
endef

$(foreach number, 01 02 03, $(call pthread_task, $(number)))
$(call openmp_task, 04)
LDFLAGS += $(CHECK_LDFLAGS)
$(call openmp_task, 05, sort.c genseq.c quicksort.c)
$(call openmp_task, 06, sort.c genseq.c mergesort.c quicksort.c)
