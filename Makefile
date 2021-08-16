.PHONY: all

all: $(shell mkdir -p build) $(shell cmake -S . -B build) $(shell make -C build) $(shell cd build && ./v_engine)
