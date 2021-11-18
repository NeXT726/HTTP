#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http.h"
#include "def.h"
#include "head.h"

int is_chunk = 1;

int copy_file(hheader *h, char * buf) 
{
//这里是为了把url中的'/'去掉，文件路径的解析依旧需要优化 //TODO
    FILE * f = fopen(h->url+1, "r");
    if(f == NULL) {
        printf("该文件不存在\n");
        return -1;
    }

//这里一次把file的内容都读出来，应该使用while依次读出，后面再改 //TODO
    int file_sz = fread(buf, 1, FILE_SZ - strlen(buf), f);
    printf("file size : %d\n", file_sz);
    return file_sz;
}

int ack_get(int sock, char *ack_buf, char *file_buf, int f_sz)
{
    if(!is_chunk){
        strcpy(ack_buf + strlen(ack_buf), file_buf);
        write(sock, ack_buf, strlen(ack_buf));
        return 1;
    }
    
    int f_now = 0;
    int one_send;
    while(f_now < f_sz){
        one_send = ((f_sz-f_now) < CHUNK_SZ) ? (f_sz - f_now) : CHUNK_SZ;
        sprintf(ack_buf + strlen(ack_buf), "%x\r\n", one_send);
        memcpy(ack_buf + strlen(ack_buf), file_buf + f_now, one_send);
        strcpy(ack_buf + strlen(ack_buf), "\r\n");
        write(sock, ack_buf, strlen(ack_buf));
        //printf("ack_buf:%ld\t", strlen(ack_buf));
        memset(ack_buf, 0, BUFFER_SZ);
        f_now = f_now + one_send;
        //printf("f_now:%d\n", f_now);
    }
    strcpy(ack_buf + strlen(ack_buf), "0\r\n\r\n");

    //printf("ack_buf:%s\n", ack_buf);
    write(sock, ack_buf, strlen(ack_buf));
    return 1;
}

//这里有严重的bug，strlen会导致字符串的解析出现问题，零值的出现很普遍，并不一定在字符串结尾。
//先以全1作为测试，后续再改TODO
int handle_get(int sock, hheader *h, rheader *r, char *data_buf)
{
    char * ack_buff = malloc(BUFFER_SZ);
    memset(ack_buff, 0, BUFFER_SZ);
    char * file_buff = malloc(FILE_SZ);
    memset(file_buff, 0, FILE_SZ);

    strcpy(ack_buff, h->version);
    strcpy(ack_buff + strlen(ack_buff), OK);
    if(is_chunk) strcpy(ack_buff + strlen(ack_buff) , IS_CHUNK);
    strcpy(ack_buff + strlen(ack_buff) , "Connection: Keep-Alive\r\n");
    printf("ack_head:%s\n", ack_buff);
    strcpy(ack_buff + strlen(ack_buff), "\r\n");

    int file_sz = copy_file(h, file_buff);
    ack_get(sock, ack_buff, file_buff, file_sz);
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
    printf("data:%s\n\n", data_buffer);

//调用函数处理请求
//目前只是实现get和post，之后实现多了可以用函数数组的方式可能更快
    int metd = get_method(headerh.method);
    if(metd == GET) handle_get(sock, &headerh, &headerr, data_buffer);
    else if(metd == POST) handle_post(sock, &headerh, &headerr, data_buffer);
    else {
        printf("该请求方法还没有被实现\n");
        return -1;
    }

    //close(sock);
    if(!is_alive(&headerr)) close(sock);
    else printf("keep-alive\n");
}
