main : client server

client : client.c common.h
	gcc client.c -o client

server : server.c common.h
	gcc server.c -o server

clean :
	rm client server