#pragma once
#include <string>
#include <string>
#include <fstream>
#include <windows.h>

std::string base64_encode(unsigned char const*, unsigned int len);      //base64编码
std::string base64_decode(std::string const& s);      //base64解码
std::string UTF8ToGBK(const char* strUTF8);           //utf8转GBK
std::string GBKToUTF8(const char* strGBK);            //GBK转utf8
