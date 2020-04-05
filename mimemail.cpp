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

    /*unsigned int sizes = subject.length();
    unsigned const char * sEncode =  (const unsigned char*)subject.c_str();
    subject = base64_encode(sEncode,sizes);
    */

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
    prepare_header();
    prepare_body();
}


MIMEMail::MIMEMail()
{
    m_sMIMEContentType =  "multipart/mixed" ;
    m_sPartBoundary =  "ZYCSDLzhangyuanchishidalao" ;
    m_sNoMIMEText = "This is a multi-part message in MIME format." ;

    //初始化m_MIMETypeList内容
    MIMEContent* pType;
    // These objects are deleted by CMIMTypeManager's destructor
    pType = new TextPlain( TEXT_PLAIN, 72 );
    register_mime_type( pType );
    pType = new AppOctetStream( APPLICATION_OCTETSTREAM );
    register_mime_type( pType );

}

//信头中加上MIME部分
void MIMEMail::prepare_header()
{
    Mail::prepare_header();
    string mimeVersion = "MIME-Version: 1.0\r\n";
    string content = "Content-Type: "+m_sMIMEContentType+";"+"boundary="+m_sPartBoundary;
    header = header + mimeVersion + content;
}

// 整理MIME信件体
void MIMEMail::prepare_body()
{
     if(body != "")
     {
         AddMIMEPart(body,TEXT_PLAIN,"",BASE64,false);
     }
     body = m_sNoMIMEText;
     body +=  "\r\n\r\n" ;
     append_mime_parts();
     insert_message_end( body );

}

//body中添加MIME部分类容（附件部分待处理）
void MIMEMail::append_mime_parts()
{

   MIMEPart part;

   /*------------------------------------------------------------------------*/
   //将filelist中的附件加入到m_MIMEPartList中
   list<string>::iterator i;
   i = fileList.begin();
   while(i!= fileList.end())
   {
       AddMIMEPart(*i,APPLICATION_OCTETSTREAM,"",BASE64,true);
       i++;
   }

   //遍历m_MIMEPartList，将每个part转换为MIME格式加入body中
   list<MIMEPart>::iterator i2;
   i2 = m_MIMEPartList.begin();
   while (i2 != m_MIMEPartList.end()) {
       part = *i2;
       MIMEContent *mimeType = NULL;

       //判断part的MIME类型
       list<MIMEContent*>::iterator i3;
       i3 = m_MIMETypeList.begin();
       while(i3!= m_MIMETypeList.end())
       {
           if((*i3)->QueryType(part.m_nContentType))
           {
             mimeType = *i3;
             break;
           }
           i3++;
       }

       insert_boundary(body);
       mimeType->AppendPart(part.m_sContent,part.m_sParameters,part.m_nEncoding,part.m_bPath,body);

       i2++;
   }

   /*---------------------------------------------------------------------------*/

   /*part = m_MIMEPartList.front();
   MIMEContent *mimeType = NULL;
   mimeType = m_MIMETypeList.front();
   insert_boundary(body);
   mimeType->AppendPart(part.m_sContent,part.m_sParameters,part.m_nEncoding,part.m_bPath,body);*/
}


// 添加MIME部分内容到m_MIMEPartList中
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

 //插入MIME部分分界线
 void MIMEMail::insert_boundary( string& sText )
 {
     string sTemp;
     if( sText.substr(sText.length()-2) !=  "\r\n"  )
     {
         sText +=  "\r\n" ;
     }
     sTemp = "--"+m_sPartBoundary+"\r\n";
     sText += sTemp;
 }


 //插入MIME部分结束线
 void MIMEMail::insert_message_end(string &body)
 {
     string sTemp;
     if( body.substr(body.length()-2) !=  "\r\n"  )
     {
         body +=  "\r\n" ;
     }
     sTemp = "--"+m_sPartBoundary+"--\r\n";
     body += sTemp;
 }



 //将m_MIMETypeList类型信息初始化
 void MIMEMail::register_mime_type( MIMEContent* pMIMEType )
 {
     if( pMIMEType == NULL )
         return;
     m_MIMETypeList.push_front(pMIMEType);
 }



