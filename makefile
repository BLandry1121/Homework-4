
CC = g++
CFLAGS = -std=c++11 -Wall

all: a.out

a.out: bowling_frame.o main.o
	$(CC) $(CFLAGS) bowling_frame.o main.o -o a.out

bowling_frame.o: bowling_frame.cpp bowling_frame.h
	$(CC) $(CFLAGS) -c bowling_frame.cpp

main.o: main.cpp bowling_frame.h
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f *.o *.out
