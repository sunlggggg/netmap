#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


void test() {
    printf("%s", "hello world\n");
}


struct sockaddr_in create_address(char *server_ip, int port) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(port);
    return serv_addr;
}

void reset(struct sockaddr_in server_address, int fd) {
    server_address.sin_family = AF_UNSPEC;
    connect(fd, (struct sockaddr *) &server_address, sizeof(server_address));
}
