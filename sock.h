#pragma once
#include<iostream>
#include<WinSock2.h>
#include<string>
#pragma comment(lib, "ws2_32.lib")

class Sock      //提供套接字服务
{
public:
	Sock();
    bool connectSocket(const char *hostID, int port);      //连接
    void sendSocket(const char *s);      //发送
    int recvSocket();      //接受
    char *getRecvBuffer();      //读缓冲区
	virtual ~Sock();

private:
    char recvBuffer[BUFSIZ];      //缓冲区
	int sock;
	int num;
	struct sockaddr_in  server;
	struct hostent *hp;
};
