main : client server

client : client.c http.h http.c
	gcc client.c http.c -o client

server : server.c http.h http.c
	gcc server.c http.c -o server

clean :
	rm -rf client server