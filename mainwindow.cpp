#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QGraphicsDropShadowEffect>
#include<QMouseEvent>
#include"pop3.h"
#include"update.h"
#include<QDebug>
#include"smtp.h"
#include<QMessageBox>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>


void MainWindow::closeEvent(QCloseEvent *event) //关闭窗口事件
{

}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    //窗口样式设置
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(Qt::black);       //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(8);        //设定阴影的模糊半径，数值越大越模糊
    ui->stackedWidget->setGraphicsEffect(effect);

    //图标设置
    QIcon icon;
    icon.addFile(":/new/prefix1/image/reply.png");
    ui->reply->setIcon(icon);

    QIcon icon2;
    icon2.addFile(":/new/prefix1/image/refresh.png");
    ui->refresh->setIcon(icon2);

    QIcon icon3;
    icon3.addFile(":/new/prefix1/image/next.png");
    ui->nextLetter->setIcon(icon3);

    QIcon icon4;
    icon4.addFile(":/new/prefix1/image/delete.png");
    ui->del->setIcon(icon4);

    QIcon icon5;
    icon5.addFile(":/new/prefix1/image/draft.png");
    ui->draft->setIcon(icon5);

    QIcon icon6;
    icon6.addFile(":/new/prefix1/image/send.png");
    ui->send->setIcon(icon6);

    QIcon icon7;
    icon7.addFile(":/new/prefix1/image/close.png");
    ui->close->setIcon(icon7);

    QIcon icon8;
    icon8.addFile(":/new/prefix1/image/refresh.png");
    ui->refresh_2->setIcon(icon8);

    QIcon icon9;
    icon9.addFile(":/new/prefix1/image/back.png");
    ui->back->setIcon(icon9);


    QIcon icon10;
    icon10.addFile(":/new/prefix1/image/back.png");
    ui->back_2->setIcon(icon10);

    QIcon icon11;
    icon11.addFile(":/new/prefix1/image/toWrite.png");
    ui->toWrite->setIcon(icon11);

    QIcon icon12;
    icon12.addFile(":/new/prefix1/image/trash.png");
    ui->toDelete->setIcon(icon12);

    QIcon icon13;
    icon13.addFile(":/new/prefix1/image/back.png");
    ui->back_3->setIcon(icon13);

    QIcon icon14;
    icon14.addFile(":/new/prefix1/image/previous.png");
    ui->previousLetter->setIcon(icon14);

    QIcon icon15;
    icon15.addFile(":/new/prefix1/image/circleNext.png");
    ui->nextHello->setIcon(icon15);

    QIcon icon16;
    icon16.addFile(":/new/prefix1/image/circleNext.png");
    ui->nextSMTP->setIcon(icon16);

    QIcon icon17;
    icon17.addFile(":/new/prefix1/image/circleNext.png");
    ui->nextPOP3->setIcon(icon17);

    QIcon icon18;
    icon18.addFile(":/new/prefix1/image/get.png");
    ui->toMainPage->setIcon(icon18);

    QIcon icon19;
    icon19.addFile(":/new/prefix1/image/cc.png");
    ui->addCC->setIcon(icon19);

    QIcon icon20;
    icon20.addFile(":/new/prefix1/image/account.png");
    ui->newAccount->setIcon(icon20);

    //QTreeWidgetItem *item4=new QTreeWidgetItem(ui->mailList);
    //ui->mailList->setItemWidget(item4,0,createItem("不爱","分隔板","22"));

    animation(ui->logo);
    animation(ui->label);
    animation(ui->label_2);

    loadAccount();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*----------------数据库读取-------------*/
void MainWindow::loadAccount()//数据库读取
{
    account.createConnection();
    account.createTable();
    QVector<Remember> temp=account.queryAll();
    accountResult=temp;

    for(QVector<Remember>::iterator i=temp.begin();i!=temp.end();i++)
    {
        ui->accountList->addItem((*i).mailAdd);
    }
}



/*------------------鼠标拖动窗口----------*/

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mouse_global_pos = event->globalPos();  //取到鼠标的绝对位置
    QPoint window_top_left = this->geometry().topLeft();  //取到窗口左上角的绝对位置

    QPoint move_point = mouse_global_pos - mouse_relative_pos;  //得到窗口左上角的绝对位置

    this->move(move_point);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint mouse_global_pos = event->globalPos();
    QPoint window_top_left = this->geometry().topLeft();
    mouse_relative_pos =  mouse_global_pos - window_top_left;  //得到鼠标相对于窗口左上角的坐标，移动时，这个值是一直不变的
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_relative_pos.setX(0);
    mouse_relative_pos.setY(0);
}

/*-------------------------------------------*/

QWidget *MainWindow::createItem(QString title, QString content,QString num)
{    //构造一个邮件条目，显示在列表中
    QWidget *myItem=new QWidget(this);
    QLabel *mainLabel=new QLabel(this);
    QLabel *subLabel=new QLabel(this);
    QLabel *numLabel=new QLabel(this);

    mainLabel->setText(title);
    subLabel->setText(content);
    numLabel->setText(num);
    numLabel->hide();

    mainLabel->setStyleSheet(QString("font: 12pt \"微软雅黑\";padding:10pt;background:transparent;"));
    subLabel->setStyleSheet(QString("font: 11pt \"微软雅黑\";padding:10pt;background:transparent;color:grey;"));

    QVBoxLayout *vLayout=new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addWidget(mainLabel);
    vLayout->addWidget(numLabel);
    vLayout->addStretch();
    vLayout->addWidget(subLabel);
    vLayout->setSpacing(5);
    vLayout->setContentsMargins(5,50,5,50);
    vLayout->setMargin(15);

    myItem->setLayout(vLayout);
    myItem->setStyleSheet(QString("QWidget:hover{background-color: rgb(243, 243, 243);}"));

    return myItem;
}



void MainWindow::on_nextHello_clicked()    //点击下一步
{
    ui->stackedWidget->setCurrentIndex(9);
}

/*-----------------------选择邮箱--------------------------*/

void MainWindow::on_MailWhu_clicked()    //点击武大邮箱
{
    ui->smtpServer->setText("smtp.whu.edu.cn");
    ui->smtpPort->setText("25");
    ui->pop3Server->setText("pop3.whu.edu.cn");
    ui->pop3Port->setText("110");
    ui->stackedWidget->setCurrentIndex(4);

    //这是我的个人信息，为了不用每次输入
    //ui->mailAddress->setText("2017301390114@whu.edu.cn");
    //ui->userName->setText("2017301390114");
    //ui->passWord->setText("789456-KK");
}

void MainWindow::on_Mail163_clicked()    //点击163邮箱
{    //自动设置好服务器
    ui->smtpServer->setText("smtp.163.com");
    ui->smtpPort->setText("25");
    ui->pop3Server->setText("pop3.163.com");
    ui->pop3Port->setText("110");
    ui->stackedWidget->setCurrentIndex(4);
}



void MainWindow::on_Mailqq_clicked()    //点击qq邮箱
{    //自动设置好服务器
    ui->smtpServer->setText("smtp.qq.com");
    ui->smtpPort->setText("25");
    ui->pop3Server->setText("pop3.qq.com");
    ui->pop3Port->setText("110");
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_MailOther_clicked()    //点击其他邮箱
{
    ui->stackedWidget->setCurrentIndex(2);
}

/*----------------------- 进入主页-----------------------------------*/
void MainWindow::on_toMainPage_clicked()    //主页按钮
{    //保存好用户输入的信息
    smtpServer=ui->smtpServer->text().toStdString();
    smtpPort=ui->smtpPort->text().toStdString();
    pop3Server=ui->pop3Server->text().toStdString();
    pop3Port=ui->pop3Port->text().toStdString();
    mailAddress=ui->mailAddress->text().toStdString();
    userName=ui->userName->text().toStdString();
    password=ui->passWord->text().toStdString();
    ui->toMainPage->setText("正在获取邮件...");
    //ui->toMainPage->setEnabled(false);

    ui->toMainPage->setEnabled(false);
    UpdateLetter();
    //ui->toMainPage->setEnabled(true);
    //ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_UpdateDone()    //在更新邮件操作完成后
{
    ui->toMainPage->setEnabled(true);
    ui->refresh->setEnabled(true);
    ui->refresh_2->setEnabled(true);
    ui->mailList->clear();    //清除邮件列表之前的内容
    cout<<"running update done!";
    ui->toMainPage->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(5);    //进入主界面
        //用新的allLetter更新邮件列表
    for (vector<readLetterSimple>::iterator i=allLetter.begin();i!=allLetter.end();i++)
    {
        QTreeWidgetItem *item3=new QTreeWidgetItem(ui->mailList);
        //qDebug()<<QString::fromLocal8Bit((*i).subject.c_str());
        //qDebug()<<QString::fromStdString((*i).subject).toUtf8();
        ui->mailList->setItemWidget(item3,0,createItem(QString::fromLocal8Bit((*i).subject.c_str()),QString::fromLocal8Bit((*i).date.c_str())+"     "+QString::fromLocal8Bit((*i).from.c_str()),QString::fromStdString((*i).displayID)));
    }
    QMessageBox::information(NULL, "提示.", "刷新成功     .");

    //下面写入数据库
    account.insert(QString::fromStdString(mailAddress),QString::fromStdString(userName),QString::fromStdString(password),QString::fromStdString(smtpServer),stoi(smtpPort.c_str()),QString::fromStdString(pop3Server),stoi(pop3Port.c_str()));

}

/*--------------------更新邮件--------------------*/

void MainWindow::on_UpdateStart()    //更新开始
{
    ui->refresh->setEnabled(false);
    ui->refresh_2->setEnabled(false);
    //QMessageBox::information(NULL, "提示.", "正在获取邮件，本操作可能耗费较长时间，视当前网络状况而定。若获取失败，请重新获取     .");
}

void MainWindow::on_UpdateWrong()    //更新出错
{
    ui->toMainPage->setEnabled(true);
    ui->refresh->setEnabled(true);
    ui->refresh_2->setEnabled(true);
    QMessageBox::critical(NULL, "错误.", "传输过程出错。请重试     .");
}

void MainWindow::UpdateLetter()    //更新所有邮件
{    //创建update线程更新allLetter
    Update *my=new Update();
    //connect(my,SIGNAL(updateDone),this,SLOT(on_UpdateDone));
    connect(my,&Update::updateDone,this,&MainWindow::on_UpdateDone);
    connect(my,&Update::updateStart,this,&MainWindow::on_UpdateStart);
    connect(my,&Update::updateWrong,this,&MainWindow::on_UpdateWrong);
    my->window=this;
    my->start();


    return;


    //下面一段是废弃代码
    User muy;
    muy.username=userName;
    muy.password=password;
    muy.hostID=pop3Server;
    muy.port=stoi(pop3Port);

    RecvMail test2(muy);
    test2.login();
    try {
        allLetter=test2.getMailList();
    } catch (...) {
cout<<"error occured!";
    }

    test2.quit();

    ui->toMainPage->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(5);
    for (vector<readLetterSimple>::iterator i=allLetter.begin();i!=allLetter.end();i++)
    {
        QTreeWidgetItem *item3=new QTreeWidgetItem(ui->mailList);
        //qDebug()<<QString::fromLocal8Bit((*i).subject.c_str());
        ui->mailList->setItemWidget(item3,0,createItem(QString::fromStdString((*i).subject),"QString::fromStdString((*i).content)",QString::fromStdString((*i).displayID)));
    }
}

/*--------------------------------------------------------------*/
void MainWindow::animation(QWidget *o)
{
    QGraphicsOpacityEffect * m_pGraphicsOpacityEffect1=new QGraphicsOpacityEffect(o);
        m_pGraphicsOpacityEffect1->setOpacity(0);
        o->setGraphicsEffect(m_pGraphicsOpacityEffect1);

    QPropertyAnimation *anime3=new QPropertyAnimation(m_pGraphicsOpacityEffect1,"opacity");
        anime3->setDuration(2000);
        anime3->setStartValue(0);
        //anime3->setKeyValueAt(0.5,1);
        anime3->setEndValue(1);
        anime3->setEasingCurve(QEasingCurve::OutSine);
        anime3->setLoopCount(1);
        anime3->start();
}



/*---------------------------显示信件--------------------------------*/
void MainWindow::on_mailList_itemDoubleClicked(QTreeWidgetItem *item, int column)    //双击查看一封信
{    //用户双击一封信，查找该邮件的id
    ui->mailList->setEnabled(false);
    QWidget *now=ui->mailList->itemWidget(item,0);
    QList<QLabel *> labelList = now->findChildren<QLabel *>();
    auto id=labelList[1]->text();
    //qDebug()<<id;
    currentLetter=id.toStdString();
    displayLetter(id);    //显示id的信
    ui->mailList->setEnabled(true);
}

void MainWindow::displayLetter(QString id)    //显示一封信
{
    //在所有的信里查找标号是id的信，然后显示
    for (vector<readLetterSimple>::iterator i=allLetter.begin();i!=allLetter.end();i++)
    {
        string thisOne=(*i).displayID;
        //cout<<thisOne<<" for serach";
        if(thisOne==id.toStdString())
        {
            ui->ReadTitle->setText(QString::fromLocal8Bit((*i).subject.c_str()));
            ui->ReadSender->setText(QString::fromLocal8Bit((*i).from.c_str()));
            ui->ReadTime->setText(QString::fromLocal8Bit((*i).date.c_str()));
            ui->ReadRecv->setText(QString::fromLocal8Bit((*i).to.c_str()));
            ui->ReadContent->setText(QString::fromLocal8Bit((*i).content.c_str()));
            ui->stackedWidget->setCurrentIndex(6);
            currentLetter=id.toStdString();
            break;
        }

    }
}



/*----------------------------删除信件----------------------------------*/

void MainWindow::removeLetter(QString id)    //删除一封信
{
        //在allletter里找到标号id的信，删除
    for (vector<readLetterSimple>::iterator i=allLetter.begin();i!=allLetter.end();i++)
    {
        string thisOne=(*i).displayID;
        //cout<<thisOne<<" for serach";
        if(thisOne==id.toStdString())
        {
            allLetter.erase(i);
            break;
        }

    }
}


void MainWindow::on_toDelete_clicked()    //点击左侧垃圾桶按钮
{
    ui->deleteList->clear();    //清空列表
    for (vector<readLetterSimple>::iterator i=allLetter.begin();i!=allLetter.end();i++)
    {    //重新从allLetter中读取所有邮件并显示
        QTreeWidgetItem *item3=new QTreeWidgetItem(ui->deleteList);
        //qDebug()<<QString::fromLocal8Bit((*i).subject.c_str());
        //qDebug()<<QString::fromStdString((*i).subject).toUtf8();
        ui->deleteList->setItemWidget(item3,0,createItem(QString::fromLocal8Bit((*i).subject.c_str()),QString::fromLocal8Bit((*i).date.c_str())+"     "+QString::fromLocal8Bit((*i).from.c_str()),QString::fromStdString((*i).displayID)));
    }
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_deleteList_itemDoubleClicked(QTreeWidgetItem *item, int column)    //删除信件时双击
{
        //先从用户双击的项中查找该邮件的id
    ui->deleteList->setEnabled(false);
    QWidget *now=ui->deleteList->itemWidget(item,0);
    QList<QLabel *> labelList = now->findChildren<QLabel *>();
    auto id=labelList[1]->text();
        //删除该邮件
    removeLetter(id);
        //还要更新allLetter的内容
    ui->deleteList->clear();    //清空列表
    for (vector<readLetterSimple>::iterator i=allLetter.begin();i!=allLetter.end();i++)
    {    //寻找标号id的邮件，删除！
        QTreeWidgetItem *item3=new QTreeWidgetItem(ui->deleteList);
        //qDebug()<<QString::fromLocal8Bit((*i).subject.c_str());
        //qDebug()<<QString::fromStdString((*i).subject).toUtf8();
        ui->deleteList->setItemWidget(item3,0,createItem(QString::fromLocal8Bit((*i).subject.c_str()),QString::fromLocal8Bit((*i).date.c_str())+"     "+QString::fromLocal8Bit((*i).from.c_str()),QString::fromStdString((*i).displayID)));
    }
    ui->stackedWidget->setCurrentIndex(8);

        //用pop3协议在服务器上把这个邮件也是拿掉
    User muy;
    muy.username=userName;
    muy.password=password;
    muy.hostID=pop3Server;
    muy.port=stoi(pop3Port);

    RecvMail test2(muy);
    test2.login();
    test2.dele(stoi(id.toStdString()));
    test2.quit();
    ui->deleteList->setEnabled(true);
}


/*----------------------------发送信件--------------------------------*/

void MainWindow::on_send_clicked()    //发送按钮
{
    ui->send->setEnabled(false);
        //构造mail对象，然后调用smtp模块发送

    test.hostID = smtpServer;
    //cout<<smtpPort<<endl;
    test.port = stoi(smtpPort);
    //cout<<"here is two"<<endl;
    test.localName = "MailClient";
    test.username = userName;
    test.password = password;
    test.mailFrom = ui->WriteSender->text().toStdString();
    test.mailTo = ui->WriteRecv->text().toStdString();
    test.subject = ui->WriteSubject->text().toLocal8Bit().toStdString();
    test.body = ui->WriteContent->toPlainText().toLocal8Bit().toStdString();
    //cout<<test.body<<endl;

        //ui->send->setText(QString::fromLocal8Bit("正在发送..."));
    QMessageBox::information(this, QString::fromLocal8Bit("Tips"), QString::fromLocal8Bit("Sending..."));
            //准备完毕，发送
    SendMail my;
        //try{
        my.sendMail(test);
        //}catch(...)
        //{
            //cout<<"Send Fail!";
        //}

        ui->stackedWidget->setCurrentIndex(5);
        //ui->send->setText(QString::fromLocal8Bit("发送"));
        ui->send->setEnabled(true);
}

/*-----------------页面跳转----------------------*/

void MainWindow::on_back_clicked()    //返回按钮
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_back_2_clicked()    //返回按钮
{
    ui->stackedWidget->setCurrentIndex(5);
    delete_all_attachment();

}

void MainWindow::on_toWrite_clicked()    //写信按钮
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->WriteSender->setText(QString::fromLocal8Bit(mailAddress.c_str()));

}

void MainWindow::on_del_clicked()    //删除按钮
{
    ui->stackedWidget->setCurrentIndex(5);
    delete_all_attachment();
}


/*--------------------刷新邮件列表---------------------------*/
void MainWindow::on_refresh_clicked()    //刷新按钮
{
    ui->refresh->setEnabled(false);
    ui->refresh_2->setEnabled(false);
        //原理：创建update线程重新获取一遍，显示在邮件列表中
    ui->stackedWidget->setCurrentIndex(5);
    Update *my=new Update();
    //connect(my,SIGNAL(updateDone),this,SLOT(on_UpdateDone));
    connect(my,&Update::updateDone,this,&MainWindow::on_UpdateDone);
    connect(my,&Update::updateStart,this,&MainWindow::on_UpdateStart);
    connect(my,&Update::updateWrong,this,&MainWindow::on_UpdateWrong);
    my->window=this;
    my->start();
}

void MainWindow::on_refresh_2_clicked()    //刷新按钮
{
    ui->refresh->setEnabled(false);
    ui->refresh_2->setEnabled(false);
    //新建一个Update线程，从服务器拉取一遍列表
    Update *my=new Update();
    //connect(my,SIGNAL(updateDone),this,SLOT(on_UpdateDone));
    connect(my,&Update::updateDone,this,&MainWindow::on_UpdateDone);
    connect(my,&Update::updateStart,this,&MainWindow::on_UpdateStart);
    connect(my,&Update::updateWrong,this,&MainWindow::on_UpdateWrong);
    my->window=this;
    my->start();
}


/*-----------------------回复邮件-----------------------------*/
void MainWindow::on_reply_clicked()    //回复按钮
{    //打开写信页面，并设置好收发件人
    QString addr=ui->ReadSender->text();
    QString subj=ui->ReadTitle->text();
    ui->WriteSender->setText(QString::fromStdString(mailAddress));
    ui->WriteRecv->setText(addr);
    ui->WriteSubject->setText("Re: "+subj);
    ui->stackedWidget->setCurrentIndex(7);

}



void MainWindow::on_nextSMTP_clicked()    //点击下一步
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_nextPOP3_clicked()    //点击下一步
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_back_3_clicked()    //返回按钮
{
    ui->mailList->clear();    //清空列表
    for (vector<readLetterSimple>::iterator i=allLetter.begin();i!=allLetter.end();i++)
    {    //从allLetter中读取邮件
        QTreeWidgetItem *item3=new QTreeWidgetItem(ui->mailList);
        qDebug()<<QString::fromLocal8Bit((*i).subject.c_str());
        //qDebug()<<QString::fromStdString((*i).subject).toUtf8();
        ui->mailList->setItemWidget(item3,0,createItem(QString::fromLocal8Bit((*i).subject.c_str()),QString::fromLocal8Bit((*i).date.c_str())+"     "+QString::fromLocal8Bit((*i).from.c_str()),QString::fromStdString((*i).displayID)));
    }
    ui->stackedWidget->setCurrentIndex(5);    //切换页面
}

void MainWindow::on_nextLetter_clicked()        //下一封
{
    displayLetter(QString::number(stoi(currentLetter)+1));
}

void MainWindow::on_previousLetter_clicked()        //上一封
{
    displayLetter(QString::number(stoi(currentLetter)-1));
}

void MainWindow::on_close_clicked()     //右上角按钮
{
    close();
}

void MainWindow::on_addCC_clicked()
{
    ui->addCC->hide();
    ui->hideFrame->hide();
}

void MainWindow::on_accountList_itemDoubleClicked(QListWidgetItem *item)
{
    QString choose=item->text();
    for(QVector<Remember>::iterator i=accountResult.begin();i!=accountResult.end();i++)
    {
        if((*i).mailAdd==choose)
        {
            smtpServer=(*i).smtpServer.toStdString();
            smtpPort=QString::number((*i).smtpPort).toStdString();
            pop3Server=(*i).pop3Server.toStdString();
            pop3Port=QString::number((*i).pop3Port).toStdString();
            mailAddress=(*i).mailAdd.toStdString();
            userName=(*i).userName.toStdString();
            password=(*i).password.toStdString();

            ui->smtpServer->setText(QString::fromStdString(smtpServer));
            ui->smtpPort->setText(QString::fromStdString(smtpPort));
            ui->pop3Server->setText(QString::fromStdString(pop3Server));
            ui->pop3Port->setText(QString::fromStdString(pop3Port));
            ui->mailAddress->setText(QString::fromStdString(mailAddress));
            ui->userName->setText(QString::fromStdString(userName));
            ui->passWord->setText(QString::fromStdString(password));

            ui->stackedWidget->setCurrentIndex(4);
            break;
        }
    }

}

void MainWindow::on_newAccount_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//添加附件
void MainWindow::on_addAttachment_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this);
    if(filePath!="")
    {
        /*if(fileList.size() == 4)
        {
            QMessageBox::information(NULL, "Warning", "最多只能添加4个附件！",QMessageBox::Yes);
            return;
        }*/

        //保存附件路径
        test.fileList.push_back(filePath.toStdString());

        //扩展高度
        if(test.fileList.size()>=4)
        {
            ui->attachFrame->setFixedSize(QSize(739,((test.fileList.size()-1)/4+1)*50));
        }

        //分割出文件名
        QString fileName = filePath.section("/",-1);
        if(fileName.length()>(fileName.mid(0,4).length()+fileName.section(".",-1).length()+1))
        {
            fileName = fileName.mid(0,4)+"…"+fileName.section(".",-1);
        }

        //设置附件图标按钮样式
        QPushButton *attachment = new QPushButton(fileName, ui->attachFrame);
        attachment->setIcon(QIcon(":/new/prefix1/image/file.png"));
        attachment->resize(150,40);
        attachment->setStyleSheet("QPushButton {border:none; font: 9pt '微软雅黑';}QPushButton:clicked{background-color: rgb(243, 243, 243);}");
        attachment->move((test.fileList.size()-1)%4*160,(test.fileList.size()-1)/4*50);

        attachment->show();
        fileButtonList.append(attachment);

        qDebug()<<"filename : "<<fileName;



    }
    qDebug()<<"filePath : "<<filePath;
}


//清空附件
void MainWindow::delete_all_attachment()
{
    if(test.fileList.size()>0)
    {
        test.fileList.clear();
        QList<QPushButton*> btns= ui->attachFrame->findChildren<QPushButton*>();
        foreach (QPushButton* btn, btns)
        {   delete btn;  }
        ui->attachFrame->setFixedSize(QSize(739,59));

    }

}




