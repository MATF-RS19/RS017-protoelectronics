PROGRAM = protoElectronics
LIB = components
CC = g++
CFLAGS = -Wall -Wextra -std=c++11

$(PROGRAM): main.o components.o
	$(CC) $(CFLAGS) -o $@ $^

components.o: components.cpp components.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.cpp components.hpp
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm *.o $(PROGRAM)


