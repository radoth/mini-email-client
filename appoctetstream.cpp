#include "appoctetstream.h"


AppOctetStream::AppOctetStream( int nContentType )
    :MIMEContent( nContentType )
{
}

AppOctetStream::~AppOctetStream()
{
}

/*----------------------------------编码部分--------------------------------------*/
//向body中添加内容
bool AppOctetStream::AppendPart(string szContent,
                                 string szParameters,
                                 int nEncoding,
                                 bool bPath,
                                 string & sDestination)
{
    QFile fAttachment(QString::fromStdString(szContent));


    if( szContent == "" )
        return false;

    if(!fAttachment.open(QIODevice::ReadOnly))
    {
        return false;
    }

    //qDebug()<<"\n"<<QString::fromStdString( sDestination)<<"\n";


    sDestination +=build_sub_header(szContent,szParameters,nEncoding,true);

    //qDebug()<<"\n"<<QString::fromStdString( sDestination)<<"\n";


    //用base64进行编码
    attach_file( &fAttachment, 4, sDestination,szContent );


    fAttachment.close();


    return true;
}



string AppOctetStream::build_sub_header(string szContent,
                                          string szParameters,
                                          int nEncoding,
                                          bool bPath)
{
    string sSubHeader;
    string sTemp;

    //截取文件名
    QString QPath = QString::fromStdString(szContent);
    QString QfileName = QPath.section("/",-1);
    string fileName = QfileName.toLocal8Bit().toStdString();

    //对文件名编码
    unsigned int subSizes = fileName.length();
    unsigned const char * sEncode1 =  (const unsigned char*)fileName.c_str();
    fileName = base64_encode(sEncode1,subSizes);

    fileName = "=?gb18030?B?"+fileName+"?=";


    if( bPath )
    {
        sTemp = "; file=\""+ fileName+"\"";
    }
    else
    {
        sTemp = "" ;
    }



    sSubHeader = "Content-Type: "+GetContentTypeString()+sTemp+"\r\n";

    sSubHeader += "Content-Transfer-Encoding: base64\r\n" ;

    sTemp = "Content-Disposition: attachment; filename=\""+fileName+"\"\r\n";
    sSubHeader+=sTemp;
    sSubHeader +="\r\n";
     const char * sEncode =  (const  char*)sSubHeader.c_str();

    sSubHeader = UTF8ToGBK(sEncode);

    return sSubHeader;

}

string AppOctetStream::GetContentTypeString()
{
    string s;
    s = "application/octet-stream" ;
    return s;
}


void AppOctetStream::attach_file(QFile* pFileAtt,
                                  int nEncoding,
                                  string & sDestination,string filePath)
{

    if( pFileAtt == NULL )
        return;


    QByteArray content;

    while(!pFileAtt->atEnd())
    {
        QByteArray temp = pFileAtt->readLine();

        QString sTemp = temp.toBase64();
        sDestination +=sTemp.toStdString();
    }

    //qDebug()<<"\n\nbodysize:\n"<<sDestination.size()<<"\n";

    sDestination +=  "\r\n" ;

}
/*------------------------------------------------------------------------------*/


/*----------------------------------解码部分--------------------------------------*/
//解码
void AppOctetStream:: DecodePart(string szContent,QByteArray &sDestination)
{
    split_header(szContent);
    //qDebug()<<"\n\nMulMIXED!!\n\n";
    //QString c = QString::fromStdString(content);



    //qDebug()<<"\n\nunDecodefileName:\n"<<QString::fromStdString(fileName)<<endl;
    //qDebug()<<"unDecodefileData:\n"<<QString::fromStdString(content)<<endl;


    QByteArray temp;
    QByteArray byteArry(content.c_str());

    temp = byteArry.fromBase64(byteArry);

    sDestination = temp;

    //sDestination = base64_decode(content.c_str());
    //qDebug()<<"\nDecodefileData:\n"<<QString::fromStdString(sDestination);
    //qDebug()<<"\nDecodefileData:\n"<<a;
}


//分割header
void AppOctetStream:: split_header(string szContent)
{

    QString qContent = QString::fromStdString(szContent);

    //将头部和主体分开
    QString qHeader =qContent.section("\r\n\r\n",0,0);
    QString qContentAppOct = qContent.section("\r\n\r\n",1);


    QString qContentType, qContentEncode, qContentDisp;

    //分析MIME段header
    QStringList qHeaderPartList = qHeader.split("\r\n");
    foreach(QString part,qHeaderPartList)
    {
        if(part.indexOf("Content-Type:")!= -1)
        {
            qContentType = part.section("Content-Type:",0).trimmed();

        }
        else if(part.indexOf("Content-Transfer-Encoding:")!= -1)
        {
            qContentEncode = part.section("Content-Transfer-Encoding:",1).trimmed();

        }
        else if(part.indexOf("Content-Disposition:")!= -1)
        {
            qContentDisp = part.section("Content-Disposition:",1).trimmed();

        }
    }

    //qDebug()<<"contentType:"<<qContentType;
    //qDebug()<<"contentEncode:"<<qContentEncode;


    //获取类型
    QString qType = qContentType.section(";",0,0);

    //获取字符集
    QString qFileName = qContentDisp.section(";",1,1).section("=",1).simplified();
    if(qFileName.section("\"",1,1)!="")
    {
        qFileName=qFileName.section("\"",1,1);
    }

    //文件名解码
    file_name_decode(qFileName);
    //qDebug()<<"\n\nunDecodefileName:\n"<<qFileName<<endl;
    //qDebug()<<"\n\nunTransfileData:\n"<<qContentAppOct<<endl;
    //qDebug()<<"encode:"<<qContentEncode;


    setHeader(qContentAppOct, qFileName ,  qContentEncode);

}


//为header中各参数赋值
void AppOctetStream:: setHeader(QString qContent, QString qFileName, QString qContentEncode)
{
    fileName = qFileName.trimmed().toStdString();
    content = qContent.trimmed().toStdString();
    encode = qContentEncode.trimmed().toStdString();
}


//文件名解码
void AppOctetStream::file_name_decode(QString &qFileName)
{

    //qDebug()<<"\n\nqSub:"<<qSub;
    QString qType =  qFileName.section("?",1,1);
    QString qEncode = qFileName.section("?",2,2);
    QString qContent = qFileName.section("?",3,-2);
    bool isChanged = false;

    //qDebug()<<"\n\nqNumb:"<<qNumb;

    string sTemp = qContent.toStdString();
    if(qEncode == "B")
    {
        //base64解码
        const char * sEncode =  (const  char*)sTemp.c_str();
        sTemp = base64_decode(sEncode);
    }
    else if(qEncode == "Q")
    {
        QPDeCoding(sTemp,isChanged);
    }

    //cout<<"\nundecode:"<<sTemp;


    if(qType=="UTF-8")
    {
        qFileName = QString::fromUtf8(sTemp.c_str());


    }
    else if(qType == "gb18030")
    {
        qFileName = QString::fromLocal8Bit(sTemp.c_str());

    }

    //qDebug()<<"fileName:"<<qFileName;


}

string AppOctetStream::GetFileName()
{
    return fileName;
}





