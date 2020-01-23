#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
	printf("%s\n", "server ...");
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	printf("%s:%d\n","fd", serv_sock);
	struct sockaddr_in serv_addr; 
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.1.102");
	// htons meaning host to network short 
	serv_addr.sin_port = htons(7023);
	bind(serv_sock, (struct sockaddr*) &serv_addr,sizeof(serv_addr));

	int ret = listen(serv_sock, 1000);

	for(;;){
		struct sockaddr_in clnt_addr;
		socklen_t clnt_addr_size = sizeof(clnt_addr);
		int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		printf("%s:%s\n","remote_ip", inet_ntoa(clnt_addr.sin_addr));
		printf("%s:%d\n","remote_port", ntohs(clnt_addr.sin_port));
		printf("%s:%d\n","fd", clnt_sock);
	}
	close(serv_sock);
	return 0;
}
