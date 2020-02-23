#pragma once
#include <string>

std::string base64_encode(unsigned char const*, unsigned int len);      //base64编码
std::string base64_decode(std::string const& s);      //base64解码
