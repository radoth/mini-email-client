#pragma once
#include<iostream>
#include<string>
#include"sock.h"
#include<regex>
#include"readLetter.h"
using namespace std;

class User      //使用pop3模块收信，要提供一个User类作为登录信息
{
public:
    string username;      //用户名
    string password;      //密码

    string hostID;      //pop3服务器
    int port;      //端口
};

class RecvMail      //提供pop3收信服务
{
public:
	RecvMail(User my);
    bool login();      //登录
    void quit();      //退出
    void noop();      //空指令
    string listAll();      //列出所有邮件
    string stat();      //STAT命令
    string reset();      //重置
    string list(int num);      //列表
    string top(int num,int row);      //TOP命令
    string uidl(int num);      //UIDL命令
    int mailCountRealTime;      //计数，现在收到了几封信
    readLetterSimple retr(int num);      //RETR命令
    vector<readLetterSimple> getMailList();      //获取所有邮件的信息
    void dele(int num);      //删信

private:
	User my;
	Sock *mysock;
	bool logined;
    string getResponse(Sock *mySock);      //获取pop3服务器应答

};
