#include "base64.h"
#include <iostream>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string base64_decode(std::string const& encoded_string) {
	size_t in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = 0; j < i; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}


string Char2Hex(unsigned char cVal)
{
    string strTemp;
    unsigned char cTempBuf1[10] = {0};
    unsigned char cTempBuf2[10] = {0};
    sprintf_s((char*)cTempBuf1, 10 * sizeof(unsigned char), "%02X", cVal);

    cTempBuf2[0] = toupper(cTempBuf1[0]);
    cTempBuf2[1] = toupper(cTempBuf1[1]);
    cTempBuf2[2] = '/0';
    strTemp = (char*)cTempBuf2;

    return strTemp;
}



// QP编码

bool QPEnCoding(string& aStr, bool &IsChanged)
{
    IsChanged = false;
    int length = (int)aStr.size();//.length();
    if (length <= 0)
    {
        return false;
    }

    unsigned char *c_copy = new unsigned char[length +1];
    strcpy_s((char*)c_copy, (length +1) * sizeof(unsigned char), aStr.c_str());
    aStr.clear();

    int nLineLen = 0;
    for (int i = 0; i < length; i++)
    {
        // c_copy[i] 介于 33 到 126 之间, 且c_copy[i]的值不为'='的时候, 直接输出
        if ((c_copy[i] >= '!') && (c_copy[i] <= '~') && (c_copy[i] != '='))
        {
            aStr += (char)c_copy[i];
            nLineLen ++;
        }
        // 其它的需编码为'='加两个字节的HEX码(大写)
        else
        {
            aStr  += "=";
            aStr  += Char2Hex(c_copy[i]);
            nLineLen += 3;
            IsChanged = true;
        }

        // 保证输出行不超过规定长度, 可在行尾加"=/r/n"序列作为软回车
        if (nLineLen >= 73)
        {
            aStr += "=/r/n";
            nLineLen = 0;
        }
    }

    // 释放内存
    delete[] c_copy;
    c_copy = NULL;

    return true;
}

// QP解码

bool QPDeCoding(string& aStr, bool &IsChanged)
{
    // 输出的字符计数
    IsChanged  = false;
    int nSrcLen = (int)aStr.size();
    if (nSrcLen <= 0)
    {
        return false;
    }

    unsigned char* pcTemp1 = 0;
    unsigned char* pDst = new unsigned char[nSrcLen +1];
    memset(pDst, 0, nSrcLen +1);
    pcTemp1 = pDst;

    char* pcTemp2 = 0;
    char* pSrc = new char[nSrcLen +1];
    strcpy_s((char*)pSrc, (nSrcLen + 1) * sizeof(char), aStr.c_str());
    aStr.clear();
    pcTemp2 = pSrc;

    int i = 0;
    while (i < nSrcLen)
    {
        // 软回车，跳过
        if (strncmp(pSrc, "=/r/n", 3) == 0)
        {
            pSrc += 3;
            i += 3;
        }
        else
        {
            // 是编码字节
            if (*pSrc == '=')
            {
                sscanf_s(pSrc, "=%02X", pDst++);
                pSrc += 3;
                i += 3;
                IsChanged = true;
            }
            // 非编码字节
            else
            {
                *pDst++ = (unsigned char)*pSrc++;
                i ++;
            }
        }
    }

    // 输出加个结束符
    char end = '\0';
    pDst = (unsigned char *)end;
    aStr = (char*)pcTemp1;

    delete[] pcTemp1;
    pcTemp1 = 0;

    delete[] pcTemp2;
    pcTemp2 = 0;

    return true;
}


 std::string UTF8ToGBK(const char* strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    std::string strTemp(szGBK);

    if (wszGBK) delete[] wszGBK;
    if (szGBK) delete[] szGBK;

    return strTemp;
}

 int isgbk(char *s, size_t ns)
 {
         if(ns > 2 && (uint8_t)*s >= 0x81 && (uint8_t)*s <= 0xfe
                 && (
                         ((uint8_t)*(s+1) >= 0x80 && (uint8_t)*(s+1) <= 0x7e)
                         || ((uint8_t)*(s+1) >= 0xa1 && (uint8_t)*(s+1) <= 0xfe)
                     )
           )
         {
                 return 1;
         }
         return 0;
 }

 int isutf8(char *s, size_t ns)
 {
         uint8_t x = 0, i = 0, j = 0, nbytes = 0, n = 0;

         for(i = 1; i < 7; i++)
         {
                 x = (uint8_t)(255 << i);
                 if(((uint8_t)*s & x) == x)
                 {
                         n = nbytes = (8 - i);
                         for(j = 0; (j < nbytes && j < ns); j++)
                         {
                                 if((uint8_t)s[j] <= 0x80 && (uint8_t)s[j] >= 0xc0)break;
                                 else n--;
                         }
                         if(n == 0) return nbytes;
                 }
         }
         return 0;
 }


 string Decode_base64(const char* Data,int DataByte,int& OutByte)
 {
     //解码表
     const char DecodeTable[] =
     {
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         62, // '+'
         0, 0, 0,
         63, // '/'
         52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
         0, 0, 0, 0, 0, 0, 0,
         0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
         13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
         0, 0, 0, 0, 0, 0,
         26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
         39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
     };
     //返回值
     string strDecode;
     int nValue;
     int i= 0;
     while (i < DataByte)
     {
         if (*Data != '\r' && *Data!='\n')
         {
             nValue = DecodeTable[*Data++] << 18;
             nValue += DecodeTable[*Data++] << 12;
             strDecode+=(nValue & 0x00FF0000) >> 16;
             OutByte++;
             if (*Data != '=')
             {
                 nValue += DecodeTable[*Data++] << 6;
                 strDecode+=(nValue & 0x0000FF00) >> 8;
                 OutByte++;
                 if (*Data != '=')
                 {
                     nValue += DecodeTable[*Data++];
                     strDecode+=nValue & 0x000000FF;
                     OutByte++;
                 }
             }
             i += 4;
         }
         else// 回车换行,跳过
         {
             Data++;
             i++;
         }
      }
     return strDecode;
 }
