CC = gcc
CFLAGS = -c -g -Wall -Wextra -std=gnu99
LFLAGS = -Wall -Wextra -pthread -g

.PHONY: all clean

all: lookup queueTest pthread-hello multi-lookup

multi-lookup: multi-lookup.o queue.o util.o
	$(CC) $(LFLAGS) $^ -o $@

lookup: lookup.o queue.o util.o
	$(CC) $(LFLAGS) $^ -o $@

queueTest: queueTest.o queue.o
	$(CC) $(LFLAGS) $^ -o $@

pthread-hello: pthread-hello.o
	$(CC) $(LFLAGS) $^ -o $@

lookup.o: lookup.c
	$(CC) $(CFLAGS) $<

multi-lookup.o: multi-lookup.c
	$(CC) $(CFLAGS) $<

queueTest.o: queueTest.c
	$(CC) $(CFLAGS) $<

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) $<

util.o: util.c util.h
	$(CC) $(CFLAGS) $<

pthread-hello.o: pthread-hello.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f lookup queueTest pthread-hello multi-lookup
	rm -f *.o
	rm -f *~
	rm -f results*
