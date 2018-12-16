TEST = test
CC = g++
CPPFLAGS = -Wall -Wextra -std=c++11 -I ../include -I ../libs



../bin/$(TEST): ../build/$(TEST).o ../build/$(TEST)-main.o  ../build/components.o ../build/log_component.o
	@ mkdir -p ../bin
	$(CC) $(CPPFLAGS) -o $@ $^

../build/log_component.o: ../src/log_component.cpp ../include/log_component.hpp 

	$(CC) $(CPPFLAGS) -c -o $@ $<

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
