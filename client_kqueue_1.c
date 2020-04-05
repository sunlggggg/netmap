#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>


const char* IP = "127.0.0.1";
const int PORT = 4312;
const int MAX_EVENT_COUNT = 5000;
const int MAX_RECV_BUFF = 65535;

int listener_;
char buf_[MAX_RECV_BUFF];

int CreateListener();
bool Register(int kq, int fd);
void WaitEvent(int kq);
void HandleEvent(int kq, struct kevent* events, int nevents);
void Accept(int kq, int connSize);
void Receive(int sock, int availBytes);
void Enqueue(const char* buf, int bytes);

int main(int argc, char* argv[])
{
    listener_ = CreateListener();
    if (listener_ == -1)
        return -1;

    int kq = kqueue();
    if (!Register(kq, listener_))
    {
        printf("%s\n", "Register listener to kq failed.\n");
        return -1;
    }

    WaitEvent(kq);

    return 0;
}

int CreateListener()
{
    isin_addr.s_addr = inet_adnt sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("%s\n","socket() failed:");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.dr(IP);
    if (bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1)
    {
        printf("%s\n","bind() failed:");
        return -1;
    }

    if (listen(sock, 5) == -1)
    {
        printf("%s\n","listen() failed:");
        return -1;
    }

    return sock;
}

bool Register(int kq, int fd)
{
    struct kevent changes[1];
    EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    int ret = kevent(kq, changes, 1, NULL, 0, NULL);
    if (ret == -1)
        return false;

    return true;
}

void WaitEvent(int kq)
{
    struct kevent events[MAX_EVENT_COUNT];
    while (true)
    {
        int ret = kevent(kq, NULL, 0, events, MAX_EVENT_COUNT, NULL);
        if (ret == -1)
        {
            printf("%s\n","kevent failed!");
            continue;
        }

        HandleEvent(kq, events, ret);
    }
}

void HandleEvent(int kq, struct kevent* events, int nevents)
{
    for (int i = 0; i < nevents; i++)
    {
        int sock = events[i].ident;
        int data = events[i].data;

        if (sock == listener_)
            Accept(kq, data);
        else
            Receive(sock, data);
    }
}

void Accept(int kq, int connSize)
{
    for (int i = 0; i < connSize; i++)
    {
        int client = accept(listener_, NULL, NULL);
        if (client == -1)
        {
            printf("%s\n","Accept failed.");
            continue;
        }

        if (!Register(kq, client))
        {
            printf("%s\n","Register client failed.");
            return;
        }
    }
}

void Receive(int sock, int availBytes)
{
    int bytes = recv(sock, buf_, availBytes, 0);
    if (bytes == 0 || bytes == -1)
    {
        close(sock);
        printf("%s\n","client close or recv failed.");
        return;
    }

    // Write buf to the receive queue.
    Enqueue(buf_, bytes);
}

void Enqueue(const char* buf, int bytes)
{
}