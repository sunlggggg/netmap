#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>

#include "util.h"

#define BUF_SIZE 1024
int otoi_map[10000];
int itoo_map[10000];
struct sockaddr_in serv_addr;
struct sockaddr_in dest_serv_addr;
int ep_fd;
struct epoll_event ep_events;

void exchange(int fd, char *buf, int len) {
    if (itoo_map[fd] != 0) {
        send(itoo_map[fd], buf, len, 0);
    } else if (otoi_map[fd] != 0) { // 外部传到内部
        if (strcmp(buf, "**RESET**\r\n") == 0) {
            printf("reset %d \n", otoi_map[fd]);
            int dest_clnt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            while (connect(dest_clnt_sock, (struct sockaddr *) &dest_serv_addr, sizeof(dest_serv_addr)) == -1);
            // 回收连接
            int t = close(otoi_map[fd]);
            printf("close result is %d\n", t);
            printf("new socket to http server is %d\n", dest_clnt_sock);
            ep_events.data.fd = dest_clnt_sock;
            epoll_ctl(ep_fd, EPOLL_CTL_ADD, dest_clnt_sock, &ep_events);
            itoo_map[dest_clnt_sock] = fd;
            otoi_map[fd] = dest_clnt_sock;
        } else {
            printf("out fd is %d\n", fd);
            send(otoi_map[fd], buf, len, 0);
        }
    }
}

void handle(struct epoll_event *ep, size_t ready) {
    char buf[13];
    for (int i = 0; i < ready; i++) {
        if (ep[i].events & EPOLLIN) {
            for (;;) {
                int fd = ep[i].data.fd;
                memset(buf, 0, sizeof(buf));
                int len = recv(fd, buf, sizeof(buf), 0);
                if (len < 0) {
                    if (len == EAGAIN) {
                        printf("%s:%d", "close ...", fd);
                        close(fd);
                        break;
                    } else {
                        return;
                    }
                }
                printf("%s", &buf);
                exchange(fd, buf, len);
                if (len < sizeof(buf)) {
                    break;
                }
            }
        }
    }
}

void client(char *server_ip, int port, char *dest_ip, int dest_port, int init_size) {
    ep_fd = epoll_create(256);
    ep_events.events = EPOLLIN | EPOLLET;
    serv_addr = create_address(server_ip, port);
    dest_serv_addr = create_address(dest_ip, dest_port);
    for (int i = 0; i < init_size; i++) {
        int clnt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        int dest_clnt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        ep_events.data.fd = clnt_sock;
        epoll_ctl(ep_fd, EPOLL_CTL_ADD, clnt_sock, &ep_events);
        ep_events.data.fd = dest_clnt_sock;
        epoll_ctl(ep_fd, EPOLL_CTL_ADD, dest_clnt_sock, &ep_events);
        while (connect(clnt_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1);
        while (connect(dest_clnt_sock, (struct sockaddr *) &dest_serv_addr, sizeof(dest_serv_addr)) == -1);
        char prefix[] = "**INNER**\r\n";
        otoi_map[clnt_sock] = dest_clnt_sock;
        itoo_map[dest_clnt_sock] = clnt_sock;
        send(clnt_sock, prefix, sizeof(prefix), 0);
    }
    int ep_size = init_size;
    struct epoll_event ep[256];
    for (;;) {
        size_t ready = epoll_wait(ep_fd, ep, ep_size, -1);
        handle(ep, ready);
    }
}

int main(int argc, char *argv[]) {
    client(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]), atoi(argv[5]));
    return 0;
}
