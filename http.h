#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "10.0.0.1"

#define HTTP_VER "HTTP/1.0"

#define CHAR_GET "GET"
#define CHAR_POST "POST"

#define GET 1
#define POST 2


int handle_request(int sock);