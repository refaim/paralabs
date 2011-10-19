.PHONY: all clean prime test

OBJS = $(foreach wc, *.pyc *.pyd *.egg-info *.so, $(wildcard $(wc)))

all: prime

clean:
	rm -rf build $(OBJS)

prime:
	cd prime && python setup.py install

test: prime
	python prime/test.py
