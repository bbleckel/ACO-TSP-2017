CPPFLAGS = -O3 -Wall -Werror
CC = g++

PROGS = main
default: $(PROGS)

main: main.o ACO.o
	$(CC) $(CPPFLAGS) -o $@ main.o ACO.o

main.o: main.cpp ACO.h
	$(CC) -c main.cpp -o $@

PSO.o: ACO.cpp ACO.h
	$(CC) -c ACO.cpp -o $@

clean:
	rm *.o
	rm -f main
