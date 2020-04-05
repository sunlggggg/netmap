#ifndef _UTIL_H
#define _UTIL_H

struct sockaddr_in create_address(char *server_ip, int port);

void test();

void reset(struct sockaddr_in server_address, int fd);

#endif