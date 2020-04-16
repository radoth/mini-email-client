#include "update.h"
#include "pop3.h"
#include <string>
#include "mainwindow.h"
#include"globaldialog.h"
#include"jasonqt_showinformationmessageboxfromotherthread.h"

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
        //QMessageBox::critical(window,"更新失败","更新时发生严重错误。");
        JasonQt_ShowInformationMessageBoxFromOtherThread::succeed=false;
        JasonQt_ShowInformationMessageBoxFromOtherThread::show("更新失败", "更新时发生严重错误。");
        cout<<"\nERROR: A fatal error occurs during transmission.\n";
    }

    //GlobalDialog *diag=new GlobalDialog();
    //diag->setContent("更新成功","更新成功结束。",true);
    JasonQt_ShowInformationMessageBoxFromOtherThread::succeed=true;
    JasonQt_ShowInformationMessageBoxFromOtherThread::show("更新成功", "更新成功结束。");
    cout<<"\nUpdate Process Ended. \n";
    test2.quit();

}
