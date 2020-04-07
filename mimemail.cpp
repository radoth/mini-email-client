#include "mimemail.h"


Mail::Mail()
{

}

Mail::Mail(string letter)
{
    this->letter = letter;
}



//非MIME格式信件体内容直接从文本框中获得
void Mail::prepare_body()
{


}


// 非MIME格式信头
void Mail::prepare_header()
{
    //string header;
    string sfrom("From:");
    from = sfrom + from + "\r\n";

    if (recv.empty())
        recv = to;
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




//带参构造函数，解码信件时调用
MIMEMail::MIMEMail(string letter):Mail(letter)
{
    DecodeTheMessage();

}

//不带参构造函数，编码邮件时调用
MIMEMail::MIMEMail()
{
    //初始化m_MIMETypeList内容
    MIMEContent* pType;
    // These objects are deleted by CMIMTypeManager's destructor
    pType = new TextPlain( TEXT_PLAIN, 72 );
    register_mime_type( pType );
    pType = new AppOctetStream( APPLICATION_OCTETSTREAM );
    register_mime_type( pType );



}


/*-----------------------------MIME编码---------------------------------------*/
//信头中加上MIME部分
void MIMEMail::prepare_header()
{
    m_sMIMEContentType =  "multipart/mixed" ;
    m_sPartBoundary =  "ZYCSDLzhangyuanchishidalao" ;
    m_sNoMIMEText = "This is a multi-part message in MIME format." ;

    Mail::prepare_header();
    string mimeVersion = "MIME-Version: 1.0\r\n";
    string content = "Content-Type: "+m_sMIMEContentType+";\r\n"+"\tboundary="+"\""+m_sPartBoundary+"\"";
    header = header + mimeVersion + content;
}

// 整理MIME信件体
void MIMEMail::prepare_body()
{
     if(body != "")
     {
         AddMIMEPart(body,TEXT_PLAIN,"charset=\"gb18030\"",BASE64,false);
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

 /*---------------------------------------------------------------------------------------*/




 /*----------------------------------MIME解码----------------------------------------------*/
 //解析邮件并展示
 void MIMEMail::DecodeTheMessage()
 {
     this->letter = letter;
     unfold();    //去折叠
     splitHeaderBody(); //分割文件头和文件尾
     headerAnalysis();    //分析信头
     bodyAnalysis();      //分析信体
 }


//分析header
void MIMEMail::headerAnalysis()
{    //分析信头，把含有信头的行查找出来，并保存
    QString qheder = QString::fromStdString(header);
    QStringList list = qheder.split("\r\n");
    foreach (QString headerPart, list) {
        if (match(headerPart.toStdString(), "Subject:(.*)", subject))
            continue;
        else if (match(headerPart.toStdString(), "Received:(.*)", received))
            continue;
        else if (match(headerPart.toStdString(), "From:(.*)", from))
            continue;
        else if (match(headerPart.toStdString(), "T[Oo]:(.*)", to))
            continue;
        else if (match(headerPart.toStdString(), "Date:(.*)", date))
            continue;
        else if (match(headerPart.toStdString(), "Content-Type:(.*)", m_sMIMEContentType))
            continue;
        else if (match(headerPart.toStdString(), "Content-Transfer-Encoding:(.*)", m_sMIMEEncode))
            continue;
        else
            continue;

    }


}


//分析body
void MIMEMail::bodyAnalysis()
{
    //分割Content-type部分
    QString qContentType = QString::fromStdString(m_sMIMEContentType).simplified();
    QString qContentEncode = QString::fromStdString(m_sMIMEEncode).simplified();

    //获取类型
    QString qType = qContentType.section(";",0,0);

    //获取参数
    QString qTypeParam = qContentType.section(";",1,1).section("=",1).simplified();
    if(qTypeParam.section("\"",1,1)!="")
    {
        qTypeParam=qTypeParam.section("\"",1,1);
    }

    //qDebug()<<"TypeParam:"<<qTypeParam;


    //获取内容
    QString qContent = QString::fromStdString(body).section("\r\n.\r\n",0,0);

    int type = judgeMIMEType(qType);

    MIMEDecode(type,qTypeParam,qContentEncode,qContent);


}


void MIMEMail::MIMEDecode(int type,QString qTypeParam,QString qContentEncode,QString qContent)
{
    switch (type) {
    case TEXT_PLAIN:
    {
        //解码
        TextPlain text(TEXT_PLAIN);
        text.setHeader(qContent,qTypeParam,qContentEncode);
        text.DecodePart(qContent.toStdString(),m_sMIMETextPlain);

        break;
    }
    case MULTIPART_ALTERNATIVE:
    {
        decodeMIMEMulAlternative(qContent,qTypeParam, qContentEncode);
        break;
    }
    case MULTIPART_MIXED:
    {
        decodeMIMEMulMixed(qContent,qTypeParam, qContentEncode);
        break;
    }
    default:
        break;

    }

}



//multipart/alternativele类型解码
void MIMEMail::decodeMIMEMulAlternative(QString qContent, QString qBoundary, QString qContentEncode)
{
    //qDebug()<<"\nalter:\n"<<qContent;

    TextPlain text(TEXT_PLAIN);
    QString qTextPlain = splitMIMEPart(qContent,qBoundary).first();
    QString qHtml = splitMIMEPart(qContent,qBoundary).last();

    text.DecodePart(qTextPlain.toStdString(),m_sMIMETextPlain);

    //qDebug()<<"textPlain:"<<qTextPlain;
    //qDebug()<<"html:"<<qHtml;


}


//multipart/mixed类型解码
void MIMEMail::decodeMIMEMulMixed(QString qContent,QString qBoundary,QString qContentEncode)
{

    QStringList qList = splitMIMEPart(qContent,qBoundary);
    foreach(QString part,qList)
    {

        QString qContentType = part.section("\r\n",0,0);
        QString qContentEncode = part.section("\r\n",1,1);

        qContentType = qContentType.section("Content-Type:",1).trimmed();
        qContentEncode = qContentEncode.section("Content-Transfer-Encoding:",1).trimmed();

        //qDebug()<<"contentType:"<<qContentType;
        //qDebug()<<"contentEncode:"<<qContentEncode;

        //获取类型
        QString qType = qContentType.section(";",0,0);
        //qDebug()<<"\n\n\n\nqType:"<<qType;


        //获取字符集
        QString qTypeParam = qContentType.section(";",1,1).section("=",1).simplified();
        if(qTypeParam.section("\"",1,1)!="")
        {
            qTypeParam=qTypeParam.section("\"",1,1);
        }

        //判断类型并解码
        if(qType == "multipart/alternative")
        {
            //qDebug()<<"qTypeParam:"<<qTypeParam;
            //qDebug()<<"contentEncode:"<<qContentEncode;
            decodeMIMEMulAlternative(part,qTypeParam, qContentEncode);
        }
        else if(qType == "text/plain")
        {
            TextPlain text(TEXT_PLAIN);
            text.DecodePart(part.toStdString(),m_sMIMETextPlain);
        }
        else if(qType == "application/octet-stream")
        {
            File file;
            file.fileData = "";
            AppOctetStream app(APPLICATION_OCTETSTREAM);
            app.DecodePart(part,file.fileData);
            file.fileName = app.GetFileName();
            //qDebug()<<"\n\nInFileName"<<QString::fromStdString(file.fileName)<<endl;
            //qDebug()<<"InFileData"<<QString::fromStdString(file.fileData)<<endl;
            attachmentList.push_back(file);
        }

        //qDebug()<<"\nqContentType:\n"<<qType<<endl;

    }
}



//MIME段分割
QStringList MIMEMail::splitMIMEPart(QString content,QString qBoundary)
{
    QStringList result;

    QString tail = "--"+qBoundary+"--";
    QString partBoundary = "--"+qBoundary;
    QString rmHeaderTailContent = content.section(partBoundary,1).section(tail,0,0);

    foreach(QString part,rmHeaderTailContent.split(partBoundary))
    {
        result.push_back(part.trimmed());
    }


    return result;
}



//分割body和header并保存
void MIMEMail::splitHeaderBody()
{
    QString letter = QString::fromStdString(this->letter);
    //qDebug()<<"letter:"<<letter<<"\n\n";


    QString sep("\r\n\r\n");
    QString header = letter.section(sep,0,0);
    //qDebug()<<"\nheader:"<<header;
    QString body = letter.section(sep,1);
    //qDebug()<<"\nbody:"<<body;

    this->header = header.toStdString();
    this->body = body.toStdString();

}



void MIMEMail::unfold()    //字符串格式处理
{
    while(1)
    {
        allocator<char>::size_type pos=letterAfter.find('\x8');
        if(pos==string::npos)
            break;
        else{
            letterAfter.erase(pos);
            letterAfter.erase(pos-1);
        }
    }
    //qDebug()<<"\nletter:"<<QString::fromStdString(letter)<<"\n";
    regex reg("\r\n\t");    //去除行折叠
    letter = regex_replace(letter, reg, "");

    regex reg2("^[+-].*?\r\n");    //去除服务器状态信息
    letter = regex_replace(letter, reg2, "");

    //cout<<"\n\nletter:\n"<<letterAfter<<endl;
}



bool MIMEMail::match(string source, const char * reg, string & destination)
{    //正则表达式匹配
    smatch result;
    regex my(reg);
    if (regex_match(source, result, my))
    {
        destination = result[1].str();
        return true;
    }
    else return false;
}


int MIMEMail::judgeMIMEType(QString contentType)
{
    if(contentType == "text/plain")
    {
        return TEXT_PLAIN;
    }
    else if(contentType == "multipart/mixed")
    {
        return MULTIPART_MIXED;
    }
    else if(contentType == "multipart/alternative")
    {
        return MULTIPART_ALTERNATIVE;
    }
    else if(contentType == "multipart/related")
    {
        return MULTIPART_RELATED;
    }
    else
    {
        return NEXT_FREE_MIME_CODE;
    }


}

void MIMEMail::debug()
{
    cout << "==================" << endl;
    cout << "subject "<<subject << endl;
    cout << "received " << received << endl;
    cout << "from " << from << endl;
    cout << "to " << to << endl;
    cout << "date " << date << endl;
    cout << "content " << body << endl;
    cout << "==================" << endl;
}


/*---------------------------------------------------------------------------------------*/
