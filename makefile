CFLAGS= -Wall -g
CC=gcc

all: beacon client
beacon: beacon.o
client: client.o
beacon.o: beacon.c
client.o: client.c

run_beacon:
	./beacon

run_client:
	./client

clean:
	rm *.o client beacon
