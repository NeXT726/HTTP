main : client server

client : client.c http.h http.c
	gcc client.c -o client

server : server.c http.h http.c
	gcc server.c -o server

clean :
	rm client server