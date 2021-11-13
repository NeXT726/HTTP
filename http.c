#include "http.h"

#define METHOD_SZ 16
#define URL_SZ 1024
#define VERSION_SZ 16

#define NAME_SZ 64
#define VALUE_SZ 256

#define BUFFER_SZ 16*1024
#define L_NM 16

typedef struct http_header_t {
    char method[METHOD_SZ];
    char url[URL_SZ];
    char version[VERSION_SZ];
} hheader;

typedef struct request_line {
    char name[NAME_SZ];
    char value[VALUE_SZ];
} r_line;

typedef struct request_header_t
{
    r_line line[L_NM];
    int l_nm;
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
    char *now = buf;
    char* next = NULL;
    char *tmp_ptr = NULL;

//将头部和数据部分分开，给data_buf赋值
    tmp_ptr = strstr(buf, "\r\n\r\n");
    *tmp_ptr = '\0';
    tmp_ptr = tmp_ptr + 4;
    strcpy(data_buf, tmp_ptr);

//将固定头部，也就是第一行解析
    next = strstr(now, "\r\n");
    if(next != NULL) {
        *next = '\0';
        next = next + 2;
    }
    tmp_ptr = strtok(now, " ");
    strcpy(h->method, tmp_ptr);
    tmp_ptr = strtok(NULL, " ");
    strcpy(h->url, tmp_ptr);
    tmp_ptr = strtok(NULL, " ");
    strcpy(h->version, tmp_ptr);
  
//解析可变长头部  
    while (next != NULL) {
        now = next;
        next = strstr(now, "\r\n") ;
        if(next != NULL) {
            *next = '\0';
            next = next + 2;
        }
        
        if(r->l_nm > L_NM) {
            printf("可变长头部过多！\n");
            return -1;
        };

        tmp_ptr = NULL;
        strtok_r(now, ":", &tmp_ptr);
        strcpy(r->line[r->l_nm].name, now);
//为了去掉可变长头部冒号后面的空格，可能有更漂亮的优化方式
        strcpy(r->line[r->l_nm].value, tmp_ptr+1);
        r->l_nm++;
    }
}

int copy_file(hheader *h, char * buf) 
{
//这里是为了把url中的'/'去掉，文件路径的解析依旧需要优化 //TODO
    FILE * f = fopen(h->url+1, "r");
    if(f == NULL) {
        printf("该文件不存在\n");
        return -1;
    }
    
//这里一次把file的内容都读出来，应该使用while依次读出，后面再改 //TODO
    int file_sz = fread(buf + strlen(buf), 1, BUFFER_SZ - strlen(buf), f);
    return 1;
}

int handle_get(int sock, hheader *h, rheader *r, char *data_buf)
{
    char * ack_buff = malloc(BUFFER_SZ);
    memset(ack_buff, 0, BUFFER_SZ);
    strcpy(ack_buff, h->version);
    strcpy(ack_buff + strlen(ack_buff), OK);
    strcpy(ack_buff + strlen(ack_buff), "\r\n");
    
    copy_file(h, ack_buff);
    printf("%s\n:", ack_buff);
    write(sock, ack_buff, strlen(ack_buff));
    return 1;
}

int handle_post(int sock, hheader *h, rheader *r, char *data_buf)
{
    FILE * f = fopen(h->url+1, "a");
    fwrite(data_buf, 1, strlen(data_buf), f);
//要记得关闭文件，否则不会真正的写入文件。
    fclose(f);

    char * ack_buff = malloc(BUFFER_SZ);
    memset(ack_buff, 0, BUFFER_SZ);
    strcpy(ack_buff, h->version);
    strcpy(ack_buff + strlen(ack_buff), OK);
    strcpy(ack_buff + strlen(ack_buff), "\r\n");
    write(sock, ack_buff, strlen(ack_buff));
    return 1;
}

int handle_request(int sock) {
    hheader headerh;
    memset(&headerh, 0, sizeof(hheader));
    rheader headerr;
    memset(&headerr, 0, sizeof(rheader));
    char *data_buffer = malloc(BUFFER_SZ);
    memset(data_buffer, 0, BUFFER_SZ);

    char *buffer = malloc(BUFFER_SZ);
    memset(buffer, 0, BUFFER_SZ);
    read(sock, buffer, BUFFER_SZ);

    parse_buffer(buffer, &headerh, &headerr, data_buffer);

    printf("method:%s\n", headerh.method);
    printf("url:%s\n", headerh.url);
    printf("version:%s\n", headerh.version);
    //printf("first name:%s\n", headerr.line[0].name);
    //printf("first value:%s\n", headerr.line[0].value);
    printf("data:%s\n", data_buffer);

//调用函数处理请求
//目前只是实现get和post，之后实现多了可以用函数数组的方式可能更快
    int metd = get_method(headerh.method);
    if(metd == GET) handle_get(sock, &headerh, &headerr, data_buffer);
    else if(metd == POST) handle_post(sock, &headerh, &headerr, data_buffer);
    else {
        printf("该请求方法还没有被实现\n");
        return -1;
    }
}

int get_request(int sock, char * url)
{
    char * ack_buff = malloc(BUFFER_SZ);
    memset(ack_buff, 0, BUFFER_SZ);
    strcpy(ack_buff, "GET djl HTTP/1.0\r\n\r\n");

    write(sock, ack_buff, strlen(ack_buff));
}