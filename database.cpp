#include "database.h"
#include"readLetter.h"
#include <QDataStream>

//建立一个数据库连接
bool DataBase::createConnection()
{
    //以后就可以用"sqlite1"与数据库进行连接了
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sqlite1");
    db.setDatabaseName(".//qtDb.db");
    if( !db.open())
    {
        qDebug() << "无法建立数据库连接.";
        return false;
    }
    return true;
}

//创建数据库表
bool DataBase::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success = query.exec("create table automobil(mailAddress text primary key,userName text,password text,smtpServer text,smtpPort int,pop3Server text,pop3Port int)");
    if(success)
    {
        qDebug() << QObject::tr("数据库表创建成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表创建失败！\n");
        return false;
    }
}

//向数据库中插入记录
bool DataBase::insert(QString mailAdd,QString userName,QString password,QString smtpServer,int smtpPort,QString pop3Server,int pop3Port)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare("insert into automobil values(?, ?, ?, ?, ?, ?, ?)");


        query.bindValue(0, mailAdd);
        query.bindValue(1, userName);
        query.bindValue(2, password);
        query.bindValue(3, smtpServer);
        query.bindValue(4, smtpPort);
        query.bindValue(5, pop3Server);
        query.bindValue(6, pop3Port);

        bool success=query.exec();
        if(!success)
        {
            QSqlError lastError = query.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("插入失败"));
            return false;
        }
    return true;
}

//查询所有信息
QVector<Remember> DataBase::queryAll()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.exec("select * from automobil");
    QSqlRecord rec = query.record();
    //qDebug() << QObject::tr("automobil表字段数：." ) << rec.count();

    QVector<Remember> back;

    while(query.next())
    {
        Remember temp;
        temp.mailAdd=query.value(0).toString();
        temp.userName=query.value(1).toString();
        temp.password=query.value(2).toString();
        temp.smtpServer=query.value(3).toString();
        temp.smtpPort=query.value(4).toInt();
        temp.pop3Server=query.value(5).toString();
        temp.pop3Port=query.value(6).toInt();
        back.push_back(temp);
    }

    return back;
}

//根据ID删除记录
bool DataBase::deleteById(int id)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("delete from automobil where id = %1").arg(id));
    if(!query.exec())
    {
        qDebug() << "删除记录失败！.";
        return false;
    }
    return true;
}

//根据ID更新记录
bool DataBase::updateById(int id)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("update automobil set attribute=?,type=?,"
                             "kind=?, nation=?,"
                             "carnumber=?, elevaltor=?,"
                             "distance=?, oil=?,"
                             "temperature=? where id=%1").arg(id));

     query.bindValue(0,"四轮");
     query.bindValue(1,"轿车");
     query.bindValue(2,"富康");
     query.bindValue(3,rand()%100);
     query.bindValue(4,rand()%10000);
     query.bindValue(5,rand()%300);
     query.bindValue(6,rand()%200000);
     query.bindValue(7,rand()%52);
     query.bindValue(8,rand()%100);

     bool success=query.exec();
     if(!success)
     {
          QSqlError lastError = query.lastError();
          qDebug() << lastError.driverText() << QString(QObject::tr("更新失败"));
     }
    return true;
}

//排序
bool DataBase::sortById()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success=query.exec("select * from automobil order by id desc");
    if(success)
    {
        qDebug() << QObject::tr("排序成功");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("排序失败！.");
        return false;
    }
}
