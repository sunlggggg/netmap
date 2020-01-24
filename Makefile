server: server.o
    cc -o server server.o
server.o: server.c
    cc -c server.c

clean:
	rm server \
        server.o
