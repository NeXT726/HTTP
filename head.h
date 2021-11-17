#ifndef HEAD_H_
    #define HEAD_H_
        #include "def.h"

        int get_method(char *method);
        int parse_buffer(char *buf, hheader *h, rheader *r, char *data_buf);
        int is_alive(rheader *r);
        
#endif