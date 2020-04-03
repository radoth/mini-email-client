#pragma once
#include "sock.h"
#include"base64.h"
#include"mimemail.h"
using namespace std;

class SendMail      //提供smtp发信服务
{
public:
    bool sendMail(MIMEMail my);

private:
    void sendAMail(MIMEMail my);      //发送一封信
    void checkError(int err);      //查错
    string generateSimpleHead(MIMEMail my);      //生成信头
    int getResponseCode(Sock *mySock);      //接受服务器反馈
};
