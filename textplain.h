#ifndef TEXTPLAIN_H
#define TEXTPLAIN_H
#include<string>
#include"mimecontent.h"
#include"base64.h"

using namespace std;

class TextPlain:public MIMEContent
{
public:
    TextPlain( int nContentType, int nWrapPos );
    virtual ~TextPlain();

    virtual bool AppendPart( string szContent, string szParameters, int nEncoding, bool bPath, string& sDestination );
    virtual string GetContentTypeString();

protected:
    int m_nWrapPos;

    string wrap_text( string szText );
    virtual string build_sub_header( string szContent, string szParameters, int nEncoding, bool bPath );
};

#endif // TEXTPLAIN_H
