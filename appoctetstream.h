#ifndef APPOCTETSTREAM_H
#define APPOCTETSTREAM_H
#include<string>
#include<iostream>
#include"mimecontent.h"
#include<QString>
#include<QFile>
#include<QDataStream>
#include"base64.h"


#define MAXBUF 58

using namespace std;

class AppOctetStream:public MIMEContent
{

public:

    AppOctetStream( int nContentType );
    virtual ~AppOctetStream();

    virtual string GetContentTypeString();
    virtual bool AppendPart( string szContent,
                             string szParameters,
                             int nEncoding,
                             bool bPath,
                             string& sDestination );
    void DecodePart(string szContent,QByteArray &sDestination);
    string  GetFileName();


protected:
    virtual void attach_file( QFile* pFileAtt, int nEncoding, string& sDestination,string filePath );
    virtual string build_sub_header( string szContent,
                                      string szParameters,
                                      int nEncoding,
                                      bool bPath );
    virtual void split_header(string szContent);
    void setHeader(QString qContent, QString qFileName, QString qContentEncode);
    void file_name_decode(QString &qFileName);




private:
    string fileName; //文件名
    string encode;  //编码方式
    string content; //内容

};

#endif // APPOCTETSTREAM_H
