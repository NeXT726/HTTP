#include "http.h"

int main() {
        struct sockaddr_in serv_addr = {0};
        struct sockaddr_in clie_addr = {0};

        int socket_sc = socket(AF_INET, SOCK_STREAM, 0);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        //不知道可不可以直接用80端口
        serv_addr.sin_port = htons(80);

        connect(socket_sc, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        char* str = malloc(99);
        memset(str, 99, 0);
        read(socket_sc, str, 100);
        printf("Client recv:%s", str);

        close(socket_sc);

        return 0;
}