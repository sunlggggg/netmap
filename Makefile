all: client server trans echo
echo: echo.o
	cc -o echo echo.o

trans: trans.o
	cc -o trans trans.o

client: client.o
	cc -o client client.o

server: server.o
	cc -o server server.o

server.o: server.c
	cc -c -std=c99 server.c

client.o: client.c
	cc -c -std=c99 client.c

trans.o: trans.c
	cc -c -std=c99 trans.c

echo.o: echo.c
	cc -c -std=c99 echo.c

clean:
	rm server \
		client \
		trans \
		echo \
        server.o \
		client.o \
		trans.o \
		echo.o
