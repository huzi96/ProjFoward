CC = gcc
CPP = g++
CFLAGS = -g -D DEBUG -Wall
LDFLAGS = -lpthread
all: server

tsh.o: tsh.c
	$(CC) -c tsh.c $(CFLAGS) 

tsh: tsh.o
	$(CC) tsh.o $(LDFLAGS) -o tsh

server: server.o nets.o
	$(CPP) server.o nets.o $(LDFLAGS) -o server

nets.o: nets.cpp
	$(CPP) -c nets.cpp $(CFLAGS)

server.o: server.cpp
	$(CPP) -c server.cpp $(CFLAGS)

sbuf.o: sbuf.cpp
	$(CPP) -c sbuf.cpp $(CFLAGS)

dev: zero.o
	$(CPP) zero.o $(LDFLAGS) -o server -lzmq

zero.o: zero.cpp
	$(CPP) -c zero.cpp $(CFLAGS)

client: client.o
	$(CPP) client.o $(CFLAGS) -lzmq -o client

client.o: client.cpp
	$(CPP) -c client.cpp $(CFLAGS)

tester: tester.cpp nets.o
	$(CPP) tester.cpp nets.o $(CFLAGS) $(LDFLAGS) -o tester

broadcast: broadcast.cpp nets_lite.o
	$(CPP) broadcast.cpp nets_lite.o -o broadcast
	
nets_lite.o: nets_lite.cpp
	$(CPP) -c nets_lite.cpp	$(CFLAGS)

clean:
	rm -f *~ *.o tsh server tester trial remote_tester