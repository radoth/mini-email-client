#ifndef MIMECONTENT_H
#define MIMECONTENT_H
#include<string>
#include"base64.h"
#include<QDebug>

using namespace std;


class MIMEContent
{
public:
    MIMEContent( int nMIMEType );
    virtual ~MIMEContent();

    bool QueryType( int nContentType );

    virtual bool AppendPart( string szContent,
                             string szParameters,
                             int nEncoding,
                             bool bPath,
                             string& sDestination ) = 0;
    virtual string GetContentTypeString() = 0;

protected:
    virtual string build_sub_header( string szContent,
                                      string szParameters,
                                      int nEncoding,
                                      bool bPath ) = 0;


private:
    int m_nMIMETypeIHandle;
};

#endif // MIMECONTENT_H
