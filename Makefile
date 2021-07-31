.PHONY: default run

default: build/Makefile
	cd build && $(MAKE)

build/Makefile: CMakeLists.txt
	mkdir -p build && cd build && cmake ..

run: default
	./build/src/spacegame
