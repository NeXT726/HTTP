#include "http.h"

#define METHOD_SZ 4
#define URL_SZ 1024
#define VERSION_SZ 16

#define BUFFER_SZ 16*1024

typedef struct http_header_t {
    char method[METHOD_SZ];
    char url[URL_SZ];
    char version[VERSION_SZ];
} hheader;

int get_method(char *method)
{
    if(!strcmp(CHAR_GET, method))
        return GET;
    else if(!strcmp(CHAR_POST, method))
        return POST;
    else return -1;
}

int handle_request(int sock) {
    char *buffer = malloc(BUFFER_SZ);
    memset(buffer, BUFFER_SZ, 0);
    read(sock, buffer, BUFFER_SZ);

    char *data_buf = strstr(buffer, "\r\n\r\n");
    data_buf = data_buf + 4;
    char *head_buf = strtok(buffer, "\r\n\r\n");

    char *sta_head = strstr(head_buf, "\r\n");
    sta_head = sta_head + 2;
    


}