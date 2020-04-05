#include "mainwindow.h"

#include "database.h"
#include <QApplication>
#include "base64.h"

int main(int argc, char *argv[])
{
    /*char* a ="zt64vbz+" ;
    string b = base64_decode(a);
    QString c = QString::fromLocal8Bit(b.c_str());
    qDebug()<<"decode:"<<c;*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
