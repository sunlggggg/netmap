#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/event.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>


#include "util.h"

// https://www.freebsd.org/cgi/man.cgi?kqueue

const int _FD_NUM = 2;


struct kevent changes[_FD_NUM];
struct kevent events[_FD_NUM];

int stdin_fd = STDIN_FILENO;
int stdout_fd = STDOUT_FILENO;

char buf[] = "hello world";

int main(int argc, char *argv[]) {
    int kq = kqueue();
    struct sockaddr_in server_address = create_address("47.94.154.37", 8047);
    struct sockaddr_in inner_server_address = create_address("47.94.154.37", 8003);
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 连接netmap server的socket
    int inner_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 连接内网服务的socket
    connect(sock_fd, (struct sockaddr *) &server_address, sizeof(server_address));
    reset(server_address,sock_fd);
    return 0;
//    connect(inner_sock_fd, (struct sockaddr *) &inner_server_address, sizeof(inner_server_address));
//
//    EV_SET(&changes[0], sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &sock_fd); // 监听来自netmap的数据
//    EV_SET(&changes[1], inner_sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &inner_sock_fd); // 监听来自netmap的数据
//
//    while (1) {
//        int nev = kevent(kq, changes, _FD_NUM, events, _FD_NUM, NULL);
//        for (int i = 0; i < nev; i++) {
//            struct kevent event = events[i];
//            int ready_fd = *((int *) event.udata);
//            if (event.filter & EVFILT_READ) {
//                char buffer[256];
//                for (;;) {
//                    memset(buffer, 0, sizeof(buffer));
//                    int len = recv(ready_fd, buffer, sizeof(buffer), 0);
//                    printf("%s", buffer);
//                    send(inner_sock_fd, buffer, sizeof(buffer), 0);
//                    if (len < sizeof(buffer)) {
//                        break;
//                    }
//                }
//            }
//        }
//    }
}

