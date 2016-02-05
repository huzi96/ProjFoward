CC = gcc
CFLAGS = -g -D DEBUG -Wall
LDFLAGS = -lpthread
all: tsh

tsh.o: tsh.c
	$(CC) -c tsh.c $(CFLAGS) 

tsh: tsh.o $(LDFLAGS)

clean:
	rm -f *~ *.o tsh