#include "http.h"

#define METHOD_SZ 4
#define URL_SZ 1024
#define VERSION_SZ 16

typedef struct http_header_t {
    char method[METHOD_SZ];
    char url[URL_SZ];
    char version[VERSION_SZ];
} hheader;

