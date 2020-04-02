#pragma once
#include "sock.h"
#include"base64.h"
#include"mimemail.h"
using namespace std;
/*
class Mail      //smtp模块发信时，提供一个mail对象作为必要的信息
{
public:
    string hostID;      //smtp服务器地址
    int port;      //端口号
    string localName;      //主机名

    string username;      //用户名
    string password;      //密码

	string mailFrom;//发信人
	string mailTo;//收信人

    string subject;      //主题
    string content;      //内容

	//以下是信头用到的信息
	string recv;//信头处的接受者
	string cc;	//抄送
	string bcc;	//秘密抄送
};
*/
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
