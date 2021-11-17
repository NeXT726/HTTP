HF := http.h head.h
CF := http.c head.c

main : client server

client : client.c $(HF) $(CF)
	gcc -o client client.c $(CF) 

server : server.c $(HF) $(CF)
	gcc -o server server.c $(CF)

clean :
	rm -rf client server