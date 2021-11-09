#include "http.h"

#define METHOD_SZ 4
#define URL_SZ 1024
#define VERSION_SZ 16

#define NAME_SZ 64
#define VALUE_SZ 1024

#define BUFFER_SZ 16*1024
#define R_NM 16

typedef struct http_header_t {
    char method[METHOD_SZ];
    char url[URL_SZ];
    char version[VERSION_SZ];
} hheader;

typedef struct request_header_t {
    char name[NAME_SZ];
    char value[VALUE_SZ];
} rheader;

int get_method(char *method)
{
    if(!strcmp(CHAR_GET, method))
        return GET;
    else if(!strcmp(CHAR_POST, method))
        return POST;
    else return -1;
}

int parse_buffer(char *buf, hheader *h, rheader *r, char *data_buf)
{
    char *header;
    header = buf;
    char *tmp_ptr;
    char *out_ptr;
    char *in_ptr;
    int in = 0;

//将头部和数据部分分开，给data_buf赋值
    strtok_r(buf, "\r\n\r\n", &out_ptr);
    data_buf = out_ptr;
//将固定头部，也就是第一行解析
    out_ptr = NULL;
    tmp_ptr = strtok_r(header, "\r\n", &out_ptr);

    tmp_ptr = strtok_r(header, " ", &in_ptr);
    strcpy(h->method, tmp_ptr);
    tmp_ptr = strtok_r(NULL, " ", &in_ptr);
    strcpy(h->url, tmp_ptr);
    tmp_ptr = strtok_r(NULL, " ", &in_ptr);
    strcpy(h->version, tmp_ptr);
//解析可变长头部
    while((tmp_ptr = strtok_r(NULL, "\r\n", &out_ptr)) != NULL){
        if(in > R_NM) {
            printf("可变长头部过多！\n");
            return -1;
        };

        in_ptr = NULL;
        strtok_r(tmp_ptr, ":", in_ptr);
        strcpy(r[in].name, tmp_ptr);
        strcpy(r[in].value, in_ptr);
        in++;
    }
}

int handle_request(int sock) {
    hheader headerh;
    rheader headerr[R_NM];
    char *data_buffer = malloc(BUFFER_SZ);
    memset(data_buffer, 0, BUFFER_SZ);

    char *buffer = malloc(BUFFER_SZ);
    memset(buffer, 0, BUFFER_SZ);
    read(sock, buffer, BUFFER_SZ);

    parse_buffer(buffer, &headerh, headerr, data_buffer);
}