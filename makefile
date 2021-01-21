PROJECT = forests
OBJECTS = struct.o parser.o main.o
SOURCES = $(OBJECTS:.o=.c)
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

.PHONY: all clean

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: 
	$(CC) $(CFLAGS) -c $<

clean:	
	rm -f *.o $(PROJECT) .depend

.depend: $(SOURCES)
	rm -f .depend
	$(CC) $(CFLAGS) -MM $^ >.depend;

include .depend
