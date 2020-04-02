#ifndef APPOCTETSTREAM_H
#define APPOCTETSTREAM_H
#include<string>
#include<iostream>
#include"mimecontent.h"
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

protected:
    //virtual void attach_file( CStdioFile* pFileAtt, int nEncoding, string& sDestination );
    virtual string build_sub_header( string szContent,
                                      string szParameters,
                                      int nEncoding,
                                      bool bPath );
};

#endif // APPOCTETSTREAM_H
