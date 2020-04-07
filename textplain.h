#ifndef TEXTPLAIN_H
#define TEXTPLAIN_H
#include<string>
#include"mimecontent.h"

using namespace std;

class TextPlain:public MIMEContent
{
public:
    TextPlain( int nContentType, int nWrapPos=72 );
    virtual ~TextPlain();

    virtual bool AppendPart( string szContent, string szParameters, int nEncoding, bool bPath, string& sDestination );
    virtual string GetContentTypeString();
    void DecodePart(string szContent,string &sDestination);
    void setHeader(QString qContent, QString qCharset, QString qContentEncode);


protected:
    //int m_nWrapPos;

    string wrap_text( string szText );
    virtual string build_sub_header( string szContent, string szParameters, int nEncoding, bool bPath );
    virtual void split_header(string szContent);


private:
    string charset; //字符集
    string encode;  //编码方式
    string content; //内容

};

#endif // TEXTPLAIN_H
