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
        //qDebug()<<"\n"<<temp<<"\n\n\n\n";
        QString sTemp = temp.toBase64();
        sDestination +=sTemp.toStdString();
        //content += temp;
    }
    //sDestination +="张原驰是大佬";
    qDebug()<<"\n\nbodysize:\n"<<sDestination.size()<<"\n";


    //QString b64Content = content.toBase64();
    //b64Content = b64Content+b64Content+b64Content+b64Content;
    //sDestination +=b64Content.toStdString();
    sDestination +=  "\r\n" ;

    //qDebug()<<"\n\n\nQByteArray"<<content<<"\n\n\n\n";

  /*  CMIMECode* pEncoder;
    int nBytesRead;
    TCHAR szBuffer[ BYTES_TO_READ + 1 ];

    ASSERT( pFileAtt != NULL );
    if( pFileAtt == NULL )
        return;
    switch( nEncoding )
    {
        // This class handles only Base64 encoding, but others
        //  may be added here.
        default:
            // Fall through to...
        case CMIMEMessage::BASE64:
            try
            {
                pEncoder = new CBase64;
            }
            catch( CMemoryException* e )
            {
                delete e;
                return;
            }
    }
    if( pEncoder == NULL )	// Old habits are hard to break
        return;
    do
    {
        try
        {
            nBytesRead = pFileAtt->Read( szBuffer, BYTES_TO_READ );
        }
        catch( CFileException* e )
        {
            delete e;
            break;
        }
        szBuffer[ nBytesRead ] = 0;	// Terminate the string
        sDestination += pEncoder->Encode( szBuffer, nBytesRead );
        sDestination += _T( "\r\n" );
    } while( nBytesRead == BYTES_TO_READ );
    sDestination += _T( "\r\n" );
    delete pEncoder;*/
}
/*------------------------------------------------------------------------------*/


/*----------------------------------解码部分--------------------------------------*/
//解码
void AppOctetStream:: DecodePart(string szContent,string &sDestination)
{
    qDebug()<<"nothing";
}

void AppOctetStream:: split_header(string szContent)
{

}


