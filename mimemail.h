#ifndef MIMEMAIL_H
#define MIMEMAIL_H
#include<string>
#include<list>
#include"textplain.h"
#include"appoctetstream.h"
#include <regex>
#include<QString>
#include<QRegExp>
#include<QDebug>
#include<iostream>

using namespace std;


class Mail      //smtp模块发信时，提供一个mail对象作为必要的信息
{
public:
    Mail();
    Mail(string letter);


    string hostID;      //smtp服务器地址
    int port;      //端口号
    string localName;      //主机名

    string username;      //用户名
    string password;      //密码

    string from;//发信人
    string to;//收信人

    string subject;      //主题
    string header;      //信头
    string body;      //内容

    //以下是信头用到的信息
    string recv;//信头处的接受者
    string cc;	//抄送
    string bcc;	//秘密抄送

    /*--------------------readLetter中新加入部分---------------------*/
    string received;      //接受主机
    string date;      //日期
    string junk;      //临时变量
    string displayID;      //邮件id，删除和查看邮件用
    /*--------------------------------------------------------------*/

    //组装信件
    void FormatTheMessage();


protected:
    virtual void prepare_header();
    virtual void prepare_body();

    /*--------------------readLetter中新加入部分---------------------*/
    //变量
    string letter;      //原始字符串
    string letterAfter;      //去除换行
    vector<string> letterFinal;      //去除信头
    /*--------------------------------------------------------------*/
};

class MIMEMail:public Mail
{
public:
    MIMEMail(string letter);
    MIMEMail();

    //解析邮件
    void DecodeTheMessage();

    string m_sNoMIMEText;    //信件体MIME格式声明语句
    string m_sPartBoundary;  //分割线内容
    string m_sMIMEContentType;//MIME段内容类型
    string m_sMIMEEncode;     //MIME邮件编码方式

    string m_sMIMETextPlain;  //纯文本内容
    string m_sMIMEHtml;       //富文本内容

    list<string>fileList;    //储存附件文件名

    // MIME Type Codes
    enum eMIMETypeCode
    {
        TEXT_PLAIN = 0,
        APPLICATION_OCTETSTREAM,
        MULTIPART_MIXED,
        MULTIPART_ALTERNATIVE,
        MULTIPART_RELATED,
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

    struct File{
        string fileName;
        QByteArray fileData;
    };

    list<File> attachmentList;


protected:
    /*-----------------MIME编码----------------------------*/
    //向m_MIMETypeList中增加类型
    void register_mime_type( MIMEContent* pMIMEType );

    //将邮件转化为MIME格式
    void append_mime_parts();

    //将信头格式化
    virtual void prepare_header();
    //将信体格式化
    virtual void prepare_body();

    //插入MIME段分割线
    void insert_boundary( string& sText );
    //插入body尾部
    void insert_message_end(string &body);


    //向m_MIMEPartList加入body的mime段
    bool AddMIMEPart( string szContent,
                      int nContentType = APPLICATION_OCTETSTREAM,
                      string szParameters = "" ,
                      int nEncoding = BASE64,
                      bool bPath = true );

    /*-----------------------------------------------------------*/


    /*------------------------MIME解析----------------------------*/
    //函数
    void unfold();      //分割字符串
    void headerAnalysis();      //分析header
    void bodyAnalysis();      //分析body
    void splitHeaderBody(); //分割header和body并保存
    bool match(string source, const char *reg, string &destination);      //匹配
    QStringList splitMIMEPart(QString content,QString qBoundary); //MIME段分割
    int judgeMIMEType(QString contentType);   //判断类型
    //void decodeMIMETextPlain(QString qContent,QString qCharset,QString qContentEncode);   //textplain类型解码
    void decodeMIMEMulAlternative(QString qContent,QString qBoundary,QString qContentEncode); //multipart/alternativele类型解码
    void decodeMIMEMulMixed(QString qContent,QString qBoundary,QString qContentEncode); //multipart/mixed类型解码

    void MIMEDecode(int type,QString qTypeParam,QString qContentEncode,QString qContent);//解码


    void debug();
    /*-----------------------------------------------------------*/



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



};



#endif // MIMEMAIL_H
