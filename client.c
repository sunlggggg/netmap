#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
void client(char *server_ip, int port){
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    // htons meaning host to network short
    serv_addr.sin_port = htons(port);
    int clnt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    while(connect(clnt_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1);
    printf("%s:%d\n", "connected fd",clnt_sock);
    char buf[BUF_SIZE];
    for(;;){
        memset(buf,0,sizeof(buf) -1);
        buf[BUF_SIZE-1] = '\0';
        int len = recv(clnt_sock, buf, sizeof(buf) -1,0);
        printf("%s", buf );
    }
}

int main(int argc, char *argv[]){
    client("192.168.1.102", atoi(argv[1]));
}
