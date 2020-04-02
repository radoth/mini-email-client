#ifndef MIMEMAIL_H
#define MIMEMAIL_H
#include<string>
#include<list>
#include"mimecontent.h"
#include"textplain.h"

using namespace std;
class Mail      //smtp模块发信时，提供一个mail对象作为必要的信息
{
public:
    string hostID;      //smtp服务器地址
    int port;      //端口号
    string localName;      //主机名

    string username;      //用户名
    string password;      //密码

    string mailFrom;//发信人
    string mailTo;//收信人

    string subject;      //主题
    string header;      //信头
    string body;      //内容

    //以下是信头用到的信息
    string recv;//信头处的接受者
    string cc;	//抄送
    string bcc;	//秘密抄送
    void FormatTheMessage();

protected:
    virtual void prepare_header();
    virtual void prepare_body();

};

class MIMEMail:public Mail
{
public:
    MIMEMail();

    string m_sPartBoundary;//分割线内容
    string m_sMIMEContentType;//MIME段内容类型

    // MIME Type Codes
    enum eMIMETypeCode
    {
        TEXT_PLAIN = 0,
        APPLICATION_OCTETSTREAM,
        NEXT_FREE_MIME_CODE
    };
    //编码方式
    enum eMIMEEncodingCode
    {
        _7BIT = 0,
        _8BIT,
        BINARY,
        QUOTED_PRINTABLE,
        BASE64,
        NEXT_FREE_ENCODING_CODE
    };

    //增加MIME部分
    bool AddMIMEPart( string szContent,
                      int nContentType = APPLICATION_OCTETSTREAM,
                      string szParameters = "" ,
                      int nEncoding = BASE64,
                      bool bPath = true );
protected:
    void register_mime_type( MIMEContent* pMIMEType );

    //插入MIME段分割线
    void insert_boundary( string& sText );
    //插入body尾部
    void insert_message_end(string &body);
    //将邮件转化为MIME格式
    void append_mime_parts();

    //将信头格式化
    virtual void prepare_header();
    //将信体格式化
    virtual void prepare_body();

    //信件体MIME格式声明语句
    string m_sNoMIMEText;

private:
    //MIME段部分
    class MIMEPart
    {
    public:
        int m_nEncoding;    //编码方式
        int m_nContentType; //类型
        string m_sParameters;
        bool m_bPath;
        string m_sContent;  //内容
    };

    //储存所有MIME段的List
    list <MIMEPart> m_MIMEPartList;

    //储存所有MIME类型
    list <MIMEContent*> m_MIMETypeList;

    class MIMETypeManager
    {
    public:
        //CMIMEContentAgent* GetHandler( int nContentType );
        //void RegisterMIMEType( MIMEContentAgent* pMIMEType);
        virtual  ~MIMETypeManager();
        MIMETypeManager();
    private:
        //CCriticalSection m_csAccess;
        //CList < CMIMEContentAgent*, CMIMEContentAgent* > m_MIMETypeList;
    };

    //static CMIMETypeManager m_MIMETypeManager;
};

#endif // MIMEMAIL_H
