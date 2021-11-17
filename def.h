#ifndef DEF_H_
    #define DEF_H_
        #define HTTP_VER "HTTP/1.0"

        #define CHAR_GET "GET"
        #define CHAR_POST "POST"
        
        #define OK " 200 OK\r\n"
        #define IS_CHUNK "Transfer-Encoding: chunked\r\n"

        #define GET 1
        #define POST 2

        #define METHOD_SZ 16
        #define URL_SZ 512
        #define VERSION_SZ 16

        #define L_NM 16
        #define NAME_SZ 64
        #define VALUE_SZ 128

        #define BUFFER_SZ 1500
        #define FILE_SZ 2*1024*1024
        #define CHUNK_SZ 512
        
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
#endif