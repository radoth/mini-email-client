#pragma once
#include <iostream>
#include <string>
#include <regex>
using namespace std;

class readLetterSimple {      //一个readLetterSimple对象包含一封信的全部信息
public:
    readLetterSimple(string letter);      //输入字符串，自动分析
    string subject;      //主题
    string received;      //接受主机
    string from;      //发件人
    string to;      //收件人
    string date;      //日期
    string junk;      //临时变量
    string content;      //正文
    string displayID;      //邮件id，删除和查看邮件用
    void debug();      //调试输出信息

private:
    string letter;      //原始字符串
    string letterAfter;      //去除换行
    vector<string> letterFinal;      //去除信头
	
    void unfold();      //分割字符串
    void analysis();      //分析字符串
    bool match(string source, const char *reg, string &destination);      //匹配

};

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);      //字符串分割
