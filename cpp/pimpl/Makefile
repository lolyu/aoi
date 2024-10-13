CC = g++
CFLAGS = -g

widget.o: widget.cpp

widget_util.o: widget_util.cpp

main.o: main.cpp

all: widget.o widget_util.o main.o
	g++ -g widget.o main.o widget_util.o -o main

clean:
	rm -f *.o
