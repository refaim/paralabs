.PHONY: all clean prime test executables

OBJS = $(foreach wc, *.pyc *.pyd *.egg-info *.so, $(wildcard $(wc)))
BUILD_DIR = build

all: clean prime

clean:
	rm -rf $(BUILD_DIR) $(OBJS) primes.lst

prime:
	cd prime && python setup.py install

test: clean prime
	python prime/test.py

$(BUILD_DIR):
	mkdir $@

executables: clean prime | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G "Unix Makefiles" .. && make
