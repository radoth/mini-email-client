#include "sock.h"
#include <stdexcept>
#include <sys/types.h> 
using namespace std;


Sock::Sock()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		throw runtime_error("套接字初始化失败\n");
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		throw runtime_error("套接字初始化失败\n");
	}
}

bool Sock::connectSocket(const char * hostID, int port)
{
	server.sin_family = AF_INET;
	hp = gethostbyname(hostID);
	if (hp == (struct hostent *)0)
	{
		throw 0;// "无法解析服务器";
		return false;
	}
	memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);
	server.sin_port = htons(port);

	

	if (connect(sock, (sockaddr *)&server, sizeof server) == -1)
	{
		throw 1; // "服务器无法连接";
		return false;
	}
	else
		return true;
}

void Sock::sendSocket(const char * s)
{
	send(sock, s, strlen(s), 0);
}

int Sock::recvSocket()
{
	memset(recvBuffer, 0, BUFSIZ);
	return recv(sock, recvBuffer, BUFSIZ, 0);
}

char * Sock::getRecvBuffer()
{
	return recvBuffer;
}


Sock::~Sock()
{
	closesocket(sock);
}

