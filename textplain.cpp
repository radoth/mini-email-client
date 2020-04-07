#include "textplain.h"

TextPlain::TextPlain( int nContentType, int nWrapPos )
    :MIMEContent( nContentType )
{
    //m_nWrapPos = nWrapPos;
}

TextPlain::~TextPlain()
{

}


/*----------------------------------编码部分--------------------------------------*/
//获得内容类型
string TextPlain::GetContentTypeString()
{
    string s;
    s = "text/plain" ;
    return s;
}

//向body中添加内容
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
    sSubHeader = "Content-Type: "+GetContentTypeString()+";\r\n\t"+szParameters+"\r\n";

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

    //base64编码
    unsigned int sizes = sTemp.length();
    unsigned const char * sEncode =  (const unsigned char*)sTemp.c_str();
    sTemp = base64_encode(sEncode,sizes);

    return sTemp;
}
/*------------------------------------------------------------------------------*/


/*----------------------------------解码部分--------------------------------------*/
//解码
void TextPlain:: DecodePart(string szContent,string &sDestination)
{
    if(content=="")
    {
        split_header(szContent);
    }
    //qDebug()<<"content:"<<qContent;
    QString qTextPlain;
    if(encode == "base64")
    {
        content = base64_decode(content.c_str());
    }
    if(charset == "gb18030")
    {
         qTextPlain = QString::fromLocal8Bit(content.c_str());
    }
    else if(charset == "UTF-8")
    {
        qTextPlain = QString::fromUtf8(content.c_str());
    }
    else
    {
        sDestination = content;
    }

    sDestination=qTextPlain.toStdString();
}


//分割MIME段头部并赋值
void TextPlain::split_header(string szContent)
{

    QString qContent = QString::fromStdString(szContent);


    QString qHeader =qContent.section("\r\n\r\n",0,0);

    QString qContentTextPlain = qContent.section("\r\n\r\n",1);


    QString qContentType = qHeader.section("\r\n",0,0);
    QString qContentEncode = qHeader.section("\r\n",1);


    qContentType = qContentType.section("Content-Type:",0).trimmed();
    qContentEncode = qContentEncode.section("Content-Transfer-Encoding:",1).trimmed();

    //qDebug()<<"contentType:"<<qContentType;
    //qDebug()<<"contentEncode:"<<qContentEncode;


    //获取类型
    QString qType = qContentType.section(";",0,0);

    //获取字符集
    QString qCharset = qContentType.section(";",1,1).section("=",1).simplified();
    if(qCharset.section("\"",1,1)!="")
    {
        qCharset=qCharset.section("\"",1,1);
    }

    /*qDebug()<<"content:"<<qContent;
    qDebug()<<"charset:"<<qCharset;
    qDebug()<<"encode:"<<qContentEncode;
    qDebug()<<"textplain"<<qContentTextPlain;
    */

    setHeader(qContentTextPlain, qCharset ,  qContentEncode);

}

void TextPlain:: setHeader(QString qContent, QString qCharset, QString qContentEncode)
{
    charset = qCharset.trimmed().toStdString();
    content = qContent.trimmed().toStdString();
    encode = qContentEncode.trimmed().toStdString();
}


