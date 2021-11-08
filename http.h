#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "10.0.0.1"

#define HTTP_VER "HTTP/1.0"

#define METHOD_GET "GET"
#define METHOD_POST "POST"
