#include "mimemail.h"

void Mail::prepare_body()
{


}
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


void Mail::FormatTheMessage()
{
    //prepare_body();
    prepare_header();
}


MIMEMail::MIMEMail()
{
    m_sMIMEContentType =  "multipart/mixed" ;
    m_sPartBoundary =  "ZYCSDLzhangyuanchishidalao" ;
    m_sNoMIMEText = "This is a multi-part message in MIME format." ;


    /*CMIMEContentAgent* pType;
    pType = new CTextPlain( TEXT_PLAIN, GetCharsPerLine() );
    register_mime_type( pType );
    pType = new CAppOctetStream( APPLICATION_OCTETSTREAM );
    register_mime_type( pType );*/
}


void MIMEMail::prepare_header()
{
    Mail::prepare_header();
    string mimeVersion = "MIME-Version: 1.0\r\n";
    string content = "Content-Type: "+m_sMIMEContentType+";"+"boundary="+m_sPartBoundary+"\r\n";
    header = header + mimeVersion + content;
}


void MIMEMail::prepare_body()
{
   /* Mail::prepare_header();
    string mimeVersion = "MIME-Version: 1.0\r\n";
    string content = "Content-Type: "+m_sMIMEContentType+";"+"boundary="+m_sPartBoundary+"\r\n";
    header = header + mimeVersion + content;*/
}
