#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;

string base64_encode(unsigned char const*, unsigned int len);      //base64编码
string base64_decode(std::string const& s);      //base64解码
string UTF8ToGBK(const char* strUTF8);           //utf8转GBK
string GBKToUTF8(const char* strGBK);            //GBK转utf8
int isgbk(char *s, size_t ns);
int isutf8(char *s, size_t ns);
string Char2Hex(unsigned char cVal);
bool QPEnCoding(string& aStr, bool &IsChanged);
bool QPDeCoding(string& aStr, bool &IsChanged);

string Decode_base64(const char* Data,int DataByte,int& OutByte);
