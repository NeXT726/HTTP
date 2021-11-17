#ifndef HTTP_H_
    #define HTTP_H_
        #define SERVER_IP "10.0.0.1"

        extern int is_chunk;

        int handle_request(int sock);
#endif