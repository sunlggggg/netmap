all: client server echo
echo: echo.o
	cc -o echo echo.o

client: client.o
	cc -o client client.o

server: server.o
	cc -o server server.o

server.o: server.c
	cc -c -std=c99 server.c

client.o: client.c
	cc -c -std=c99 client.c

echo.o: echo.c
	cc -c -std=c99 echo.c

clean:
	rm server \
		client \
		echo \
        server.o \
		client.o \
		echo.o
