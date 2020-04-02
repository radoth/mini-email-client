#include "mimemail.h"

//非MIME格式信件体内容直接从文本框中获得
void Mail::prepare_body()
{


}

// 非MIME格式信头
void Mail::prepare_header()
{
    //string header;
    string from("From:");
    from = from + mailFrom + "\r\n";

    if (recv.empty())
        recv = mailTo;
    //默认是mailto处的发件人

    string to("TO:");
    to = to + recv + "\r\n";

    string cc("");
    if (!cc.empty())
    {
        cc = "Cc:" + cc + "\r\n";
    }

    string bcc("");
    if (!bcc.empty())
    {
        bcc = "Bcc:" + bcc + "\r\n";
    }

    string dat("");
    //待完善

    string subj("");
    if (!subject.empty())
    {
        subj = "Subject:" + subject + "\r\n";
    }

    header  =from + to + cc + bcc + dat + subj;

}

//整理信件格式
void Mail::FormatTheMessage()
{
    prepare_body();
    prepare_header();
}


MIMEMail::MIMEMail()
{
    m_sMIMEContentType =  "multipart/mixed" ;
    m_sPartBoundary =  "ZYCSDLzhangyuanchishidalao" ;
    m_sNoMIMEText = "This is a multi-part message in MIME format." ;

}

//信头中加上MIME部分
void MIMEMail::prepare_header()
{
    Mail::prepare_header();
    string mimeVersion = "MIME-Version: 1.0\r\n";
    string content = "Content-Type: "+m_sMIMEContentType+";"+"boundary="+m_sPartBoundary+"\r\n";
    header = header + mimeVersion + content;
}

// 整理MIME信件体
void MIMEMail::prepare_body()
{
   /* Mail::prepare_header();
    string mimeVersion = "MIME-Version: 1.0\r\n";
    string content = "Content-Type: "+m_sMIMEContentType+";"+"boundary="+m_sPartBoundary+"\r\n";
    header = header + mimeVersion + content;*/
}

// 添加MIME部分内容
// 默认类型为 application/ oct_stream; 默认编码方式为Base64
 bool MIMEMail::AddMIMEPart(string szContent, int nContentType, string szParameters, int nEncoding, bool bPath)
 {

     MIMEPart part;
     part.m_nContentType = nContentType;
     part.m_sParameters = szParameters;
     part.m_nEncoding = nEncoding;
     part.m_bPath = bPath;
     part.m_sContent = szContent;
     //part.m_sContent.TrimLeft();
     //part.m_sContent.TrimRight();
     if( nContentType == TEXT_PLAIN )
         m_MIMEPartList.push_front(part);
     else
         m_MIMEPartList.push_back(part);
     return true;

 }
