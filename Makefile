all: client server trans
trans: trans.o
	cc -o trans trans.o

client: client.o
	cc -o client client.o

server: server.o
	cc -o server server.o

server.o: server.c
	cc -c server.c

client.o: client.c
	cc -c client.c

trans.o: trans.c
	cc -c trans.c

clean:
	rm server \
		client \
		trans \
        server.o \
		client.o \
		trans.o \
