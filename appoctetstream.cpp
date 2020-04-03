#include "appoctetstream.h"


AppOctetStream::AppOctetStream( int nContentType )
    :MIMEContent( nContentType )
{
}

AppOctetStream::~AppOctetStream()
{
}

bool AppOctetStream::AppendPart(string szContent,
                                 string szParameters,
                                 int nEncoding,
                                 bool bPath,
                                 string & sDestination)
{
    QFile fAttachment(QString::fromStdString(szContent));

    if( szContent == "" )
        return false;
    if( szContent == "" )
        return false;

    if(!fAttachment.open(QIODevice::ReadOnly))
    {
        return false;
    }

    sDestination +=build_sub_header(szContent,szParameters,nEncoding,true);

    //用base64进行编码
    attach_file( &fAttachment, 4, sDestination,szContent );

    fAttachment.close();

    //cout<<"attachment send!";
   /* CStdioFile fAttachment;

    ASSERT( szContent != NULL );
    // This class handles only file attachments, so
    // it ignores the bPath parameter.
    if( szContent == NULL )
        return false;
    if( !fAttachment.Open( szContent, (CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) ) )
        return false;
    sDestination += build_sub_header( szContent,
                                      szParameters,
                                      nEncoding,
                                      TRUE );
    attach_file( &fAttachment, CMIMEMessage::BASE64, sDestination );
    fAttachment.Close();*/
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
    string fileName = QfileName.toStdString();

    if( bPath )
    {
        sTemp = "; file="+ fileName;
    }
    else
    {
        sTemp = "" ;
    }

    sSubHeader = "Content-Type: "+GetContentTypeString()+sTemp+"\r\n";

    sSubHeader += "Content-Transfer-Encoding: base64\r\n" ;

    sTemp = "Content-Disposition: attachment; filename="+fileName+"\r\n";
    sSubHeader+=sTemp;
    sSubHeader +="\r\n";


    return sSubHeader;



    /*
    string sSubHeader;
    string sTemp;
    TCHAR szFName[ _MAX_FNAME ];
    TCHAR szExt[ _MAX_EXT ];

    _tsplitpath( szContent, NULL, NULL, szFName, szExt );

    // This class ignores szParameters and nEncoding.
    // It controls its own parameters and only handles
    // Base64 encoding.
    if( bPath )
        sTemp.Format( "; file=%s%s", szFName, szExt );
    else
        sTemp = _T( "" );
    sSubHeader.Format( _T( "Content-Type: %s%s\r\n" ),
                        (string)GetContentTypeString(),
                        (string)sTemp );
    sSubHeader += _T( "Content-Transfer-Encoding: base64\r\n" );
    sTemp.Format( _T( "Content-Disposition: attachment; filename=%s%s\r\n" ),
                  szFName, szExt );
    sSubHeader += sTemp;
    // Signal end of sub-header.
    sSubHeader += _T( "\r\n" ); // Warning: numerous concatenations
                                // are inefficient.
    return sSubHeader;
    */
}

string AppOctetStream::GetContentTypeString()
{
    string s;
    s = "application/octet-stream" ;
    return s;
}

// Caller is responsible for opening and closing the file
void AppOctetStream::attach_file(QFile* pFileAtt,
                                  int nEncoding,
                                  string & sDestination,string filePath)
{

    if( pFileAtt == NULL )
        return;


    QByteArray content =  pFileAtt->readAll();
    QString b64Content = content.toBase64();
    sDestination +=b64Content.toStdString();
    sDestination +=  "\r\n" ;


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

