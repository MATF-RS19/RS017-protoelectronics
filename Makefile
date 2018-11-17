PROGRAM = protoElectronics
LIB = components
CC = g++
CPPFLAGS = -Wall -Wextra -std=c++11

$(PROGRAM): main.o components.o logical_components.o
	$(CC) $(CPPFLAGS) -o $@ $^

components.o: components.cpp components.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

logical_components.o: logical_components.cpp logical_components.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

main.o: main.cpp components.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm *.o $(PROGRAM)


