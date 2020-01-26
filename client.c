#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

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
    char buf[13];
    for(;;){
        memset(buf,0,sizeof(buf));
        int len = recv(clnt_sock, buf, sizeof(buf),0);
        printf("%s", buf);
    }
}

int main(){
    client("192.168.1.102", 7000);
}
