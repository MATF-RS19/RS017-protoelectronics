PROGRAM = protoElectronics
CC = g++
CPPFLAGS = -Wall -Wextra -ggdb3 -std=c++14 -I include

bin/$(PROGRAM): build/main.o build/components.o build/logical_components.o
	@ mkdir -p bin
	$(CC) $(CPPFLAGS) -o $@ $^

build/components.o: src/components.cpp include/components.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

build/logical_components.o: src/logical_components.cpp include/logical_components.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

build/main.o: src/main.cpp include/components.hpp
	@ mkdir -p build
	$(CC) $(CPPFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm -rf build bin/$(PROGRAM) 
