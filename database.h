#ifndef DATABASE_H
#define DATABASE_H

#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>

class Remember
{
public:
    QString mailAdd;
    QString userName;
    QString password;
    QString smtpServer;
    int smtpPort;
    QString pop3Server;
    int pop3Port;
};

class DataBase
{
public:
    bool createConnection();  //创建一个连接
    bool createTable();       //创建数据库表
    bool insert(QString mailAdd, QString userName, QString password, QString smtpServer, int smtpPort, QString pop3Server, int pop3Port);            //出入数据
    QVector<Remember> queryAll();          //查询所有信息
    bool updateById(int id);  //更新
    bool deleteById(int id);  //删除
    bool sortById();          //排序
};



#endif // DATABASE_H
