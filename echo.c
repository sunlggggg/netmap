#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

// establish queue size
#define BACKLOG 256
#define MAX_OPEN_FD 256


void server(char *server_ip, int port){
    printf("%s\n", "server ...");
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("%s:%d\n","fd", serv_sock);
    struct sockaddr_in serv_addr; 
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    // htons meaning host to network short 
    serv_addr.sin_port = htons(port);
    bind(serv_sock, (struct sockaddr*) &serv_addr,sizeof(serv_addr));

    listen(serv_sock, BACKLOG);

    int ep_fd = epoll_create(MAX_OPEN_FD);

    struct epoll_event ep_events;
    ep_events.events = EPOLLIN|EPOLLET;
    ep_events.data.fd = serv_sock;
    epoll_ctl(ep_fd, EPOLL_CTL_ADD, serv_sock, &ep_events);
    struct epoll_event ep[MAX_OPEN_FD];

    for(;;){
        // -1 meaning blocking
        size_t ready = epoll_wait(ep_fd,ep,MAX_OPEN_FD, -1);
        char buf[13];
        for( int i = 0 ; i < ready; i++){
            // new connect
            if(ep[i].events & EPOLLIN){
                if(ep[i].data.fd == serv_sock){
                    struct sockaddr_in clnt_addr;
                    socklen_t clnt_addr_size = sizeof(clnt_addr);
                    int clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
                    printf("%s:%s\n","remote_ip", inet_ntoa(clnt_addr.sin_addr));
                    printf("%s:%d\n","remote_port", ntohs(clnt_addr.sin_port));
                    printf("%s:%d\n","fd", clnt_sock);
                    struct epoll_event tep;
                    tep.events = EPOLLIN|EPOLLET;
                    tep.data.fd = clnt_sock;
                    epoll_ctl(ep_fd,EPOLL_CTL_ADD,clnt_sock,&tep);
                } else {
                    for(;;){
                        int fd = ep[i].data.fd;
                        memset(buf,0,sizeof(buf));
                        int len = recv(fd, buf, sizeof(buf), 0);
                        if(len < 0 ){
                            if(len == EAGAIN ){
                                printf("%s:%d", "close ...", fd);
                                close(fd);
                                break;
                            } else {
                                return ;
                            }
                        } 
                        printf("%s", &buf);
                        send(fd, buf, len, 0);
                        if(len < sizeof(buf)){
                            break;
                        }
                    }
                }
            } else {
                close(ep[i].data.fd);
                printf("%s:%d\n", "close", ep[i].data.fd);
            }
        }
    }
    close(serv_sock);
}
int main(int argc,char *argv[]){
    server(argv[1], atoi(argv[2]));
    return 0;
}

