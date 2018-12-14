TEST = test
CC = g++
CPPFLAGS = -Wall -Wextra -std=c++11 -I ../include



../bin/$(TEST): ../build/$(TEST).o ../build/$(TEST)-main.o  ../build/components.o
	@ mkdir -p ../bin
	$(CC) $(CPPFLAGS) -o $@ $^

../build/components.o: ../src/components.cpp ../include/components.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

../build/$(TEST).o: $(TEST).cpp ../src/components.cpp ../include/components.hpp
	@ mkdir -p ../build
	$(CC) $(CPPFLAGS) -c -o $@ $<

../build/$(TEST)-main.o: $(TEST)-main.cpp
	@ mkdir -p ../build
	$(CC) $(CPPFLAGS) -c -o $@ $<



.PHONY: clean

clean:
	rm -rf ../build ../bin/$(TEST)
