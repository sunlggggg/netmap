#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

void trans(char *host_from, int port1, char *host_to, int port2){
    struct sockaddr_in serv_addr1, serv_addr2;
    memset(&serv_addr1,0,sizeof(serv_addr1));
    serv_addr1.sin_family = AF_INET;
    serv_addr1.sin_addr.s_addr = inet_addr(host_from);
    serv_addr1.sin_port = htons(port1);
    memset(&serv_addr2,0,sizeof(serv_addr2));
    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_addr.s_addr = inet_addr(host_to);
    serv_addr2.sin_port = htons(port2);
    int clnt_sock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int clnt_sock2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    while(connect(clnt_sock1,(struct sockaddr*)&serv_addr1,sizeof(serv_addr1)) == -1);
    while(connect(clnt_sock2,(struct sockaddr*)&serv_addr2,sizeof(serv_addr2)) == -1);
    printf("%s %s %s:%d\n", "connected",host_from,"fd", clnt_sock1);
    printf("%s %s %s:%d\n", "connected",host_to,"fd", clnt_sock2);
    char buf[13];
    for(;;) {
        memset(buf,0,sizeof(buf));
        int len = recv(clnt_sock1, buf, sizeof(buf), 0);
        send(clnt_sock2, buf, len, 0);
    }
}
int main(){
    trans("192.168.1.102", 7000, "192.168.1.102", 8000); 
    return 0;
}
