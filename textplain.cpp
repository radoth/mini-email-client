#include "textplain.h"

TextPlain::TextPlain( int nContentType, int nWrapPos )
    :MIMEContent( nContentType )
{
    //m_nWrapPos = nWrapPos;
}

TextPlain::~TextPlain()
{

}

//获得内容类型
string TextPlain::GetContentTypeString()
{
    string s;
    s = "text/plain" ;
    return s;
}

bool TextPlain::AppendPart(string szContent,
                            string szParameters,
                            int nEncoding,
                            bool bPath,
                            string & sDestination)
{
    string sSubHeader;
    string sWrapped;
    sSubHeader = build_sub_header( szContent,
                                   szParameters,
                                   nEncoding,
                                   bPath );
    sWrapped = wrap_text( szContent );
    sDestination += (sSubHeader + sWrapped);
    return true;
}

//构建邮件体部分MIME头部
string TextPlain::build_sub_header(string szContent,
                                     string szParameters,
                                     int nEncoding,
                                     bool bPath)
{
    string sSubHeader;
    sSubHeader = "Content-Type: "+GetContentTypeString()+szParameters+"\r\n";

    sSubHeader += "Content-Transfer-Encoding: " ;
    switch( nEncoding )
    {
        // 纯文本内容用base64编码
        default:
            //Fall through to...
        case 0:
            sSubHeader += "base64" ;
    }
    sSubHeader +=  "\r\n\r\n" ;
    return sSubHeader;
}

string TextPlain::wrap_text(string szText)
{

    string sTemp;

    if( szText == "" )
        return sTemp;
    sTemp = szText;
    unsigned int sizes = sTemp.length();

    unsigned const char * sEncode =  (const unsigned char*)sTemp.c_str();

    sTemp = base64_encode(sEncode,sizes);

    return sTemp;
}
