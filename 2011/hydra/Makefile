.PHONY: all clean executables

OBJS = $(wildcard *.pyc)
BUILD_DIR = build

all: executables

clean:
	$(RM) -r $(BUILD_DIR) $(OBJS) primes.lst

$(BUILD_DIR):
	mkdir $@

executables: clean | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G "Unix Makefiles" .. && make
