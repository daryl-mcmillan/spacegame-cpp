default:
	cd build && $(MAKE)

remake:
	cd build && cmake .. && $(MAKE)

run: default
	./build/spacegame
