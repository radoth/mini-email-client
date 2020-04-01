#include "smtp.h"
#include "mimemail.h"

bool SendMail::sendMail(Mail my)    //发送一封邮件
{
    try {
        cout << "The mail is begin sent..." << endl;
        sendAMail(my);
        cout << "Mail sent successfully." << endl;
        return true;
    }

    catch(int err)
    {
        cout<<"Oops! An SMTP error occurs."<<endl;
        cout<<"SMTP errCode:"<<err<<endl;
        return false;
    }


}

void SendMail::sendAMail(Mail my)
{
    Sock mysock;
    int resCode;        //建立连接
    if (mysock.connectSocket(my.hostID.c_str(), my.port) == false)
        throw 2;//建立连接失败

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;

    string helo("HELO ");    //问候
    helo = helo + my.localName + "\r\n";
    mysock.sendSocket(helo.c_str());

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);

    string login("AUTH LOGIN\r\n");    //请求登录
    mysock.sendSocket(login.c_str());

    //cout << "login" << endl;
    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);
    //if (resCode != 334)
        //throw 4;

    string usernameBase64=base64_encode(reinterpret_cast<const unsigned char*>(my.username.c_str()), my.username.length());
    mysock.sendSocket(usernameBase64.c_str());    //用户名
    mysock.sendSocket("\r\n");

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);
    //if (resCode != 334)
        //throw 5;

    string passwordBase64= base64_encode(reinterpret_cast<const unsigned char*>(my.password.c_str()), my.password.length());
    mysock.sendSocket(passwordBase64.c_str());        //密码
    mysock.sendSocket("\r\n");

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);
    //if (resCode != 235)
        //throw 6;

    mysock.sendSocket("MAIL FROM:<");    //发送发件人
    mysock.sendSocket(my.mailFrom.c_str());
    mysock.sendSocket(">");
    mysock.sendSocket("\r\n");

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);
    //if (resCode != 250)
        //throw 7;

    mysock.sendSocket("RCPT TO:<");
    mysock.sendSocket(my.mailTo.c_str());    //发送收件人
    mysock.sendSocket(">");
    mysock.sendSocket("\r\n");

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);
    //if (resCode != 250)
        //throw 8;

    mysock.sendSocket("DATA\r\n");    //发送数据

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);
    //if (resCode != 354)
        //throw 9;

    //string all = generateSimpleHead(my);

     my.FormatTheMessage();
    string all = my.header +"\r\n\r\n"+ my.body + "\r\n";

    mysock.sendSocket(all.c_str());    //发送数据
    mysock.sendSocket(".\r\n");

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);
    //if (resCode != 250)
        //throw 10;

    mysock.sendSocket("QUIT\r\n");    //退出

    resCode = getResponseCode(&mysock);
    //cout << resCode << endl;
    checkError(resCode);

}

void SendMail::checkError(int err)    //错误代码检查，待完善
{
    switch (err)
    {
    case 420:
    case 421:
    case 422:
    case 431:
    case 432:
    case 441:
    case 442:
    case 446:
    case 449:
    case 450:
    case 451:
    case 452:
    case 465:
    case 471:
    case 500:
    case 502:
    case 503:
    case 504:
    case 505:
    case 510:
    case 511:
    case 512:
    case 513:
    case 523:
    case 530:
    case 541:
    case 544:
    case 550:
    case 552:
    case 553:
    case 554:
    case 571:throw err; break;
    default:break;
    }
}
/*
string SendMail::generateSimpleHead(Mail my)    //生成信头
{
    string header;
    string from("From:");
    from = from + my.mailFrom + "\r\n";

    if (my.recv.empty())
        my.recv = my.mailTo;
    //默认是mailto处的发件人

    string to("TO:");
    to = to + my.recv + "\r\n";

    string cc("");
    if (!my.cc.empty())
    {
        cc = "Cc:" + my.cc + "\r\n";
    }

    string bcc("");
    if (!my.bcc.empty())
    {
        bcc = "Bcc:" + my.bcc + "\r\n";
    }

    string dat("");
    //待完善

    string subject("");
    if (!my.subject.empty())
    {
        subject = "Subject:" + my.subject + "\r\n";
    }

    header  =from + to + cc + bcc + dat + subject;
    string mimeVersion;
    mimeVersion = "MIME-Version: 1.0\r\n";
    //string content = "Content-Type: "+m_sMIMEContentType+";"+"boundary="+m_sPartBoundary+"\r\n";
    //string a = MIMEMail::m_sMIMEContentType

    return header;
}*/

int SendMail::getResponseCode(Sock * mySock)
{
    mySock->recvSocket();
    char *result=mySock->getRecvBuffer();
    string res(result);
    //cout << res << endl;
    //system("PAUSE");
    int RspsCode;
    try {
        RspsCode=stoi(res.substr(0, 3), 0, 10);
    } catch (...) {
        RspsCode=-1;
    }
    return RspsCode;

}
