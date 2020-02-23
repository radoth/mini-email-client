#include "update.h"
#include "pop3.h"
#include <string>
#include "mainwindow.h"
#include <QMessageBox>
void Update::run()
{
    //User对象是pop3模块需要的
    User muy;
    muy.username=((MainWindow *)window)->userName;
    muy.password=((MainWindow *)window)->password;
    muy.hostID=((MainWindow *)window)->pop3Server;
    muy.port=stoi(((MainWindow *)window)->pop3Port);

    //登录，收信，退出
    RecvMail test2(muy);
    test2.login();
    try{
        emit updateStart();
         //QMessageBox::information(NULL, QString::fromLocal8Bit("提示."), QString::fromLocal8Bit("即将获取邮件，本操作可能耗费较长时间，视当前网络状况而定。若获取失败，请重新获取."));
    ((MainWindow *)window)->allLetter=test2.getMailList();
    emit updateDone();
    }
    catch(...)
    {
        emit updateWrong();
        cout<<"!!!!!!!!!!!!!!!!!!!!";
    }
    cout<<"获取完毕.";
    test2.quit();

}
