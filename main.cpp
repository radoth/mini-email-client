#include "mainwindow.h"

#include "database.h"
#include <QApplication>
#include "base64.h"

int main(int argc, char *argv[])
{
    /*char* a ="56ys5Y2B5LqM5bCBL+atpuWkp+mCrueusee9kemhteeJiC/luKbpmYTku7Y8YnI+Cjxicj4KPGJyPg==" ;
    string b = base64_decode(a);
    QString c = QString::fromUtf8(b.c_str());
    qDebug()<<"decode:"<<c;*/

    /*bool isChanged = false;
    string b ="24=D00=A10=CA0=B10=BC0=C60=CA0=FD0=B70=A80=200-=200=B80=B10=B10=BE0.docx";
    QPDeCoding(b,isChanged);
    QString c = QString::fromLocal8Bit(b.c_str());
    qDebug()<<"decode:"<<c;*/


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();



    /*QApplication a(argc, argv);
    QWidget w;
    AttachmentButton b(&w);
    b.setText("Button");
    w.show();
    return a.exec();*/

    /*
    QString filePath = "C:/Users/10694/Desktop/test/24小时计数法 - 副本";
    QFile file(filePath);
    QIODevice::OpenMode flags = QIODevice::WriteOnly;

    string c = "";

    QByteArray ddd;

    qDebug()<<"write:"<<file.write();
    file.close();


*/


/*
    QFile file("C:/Users/10694/Desktop/abcdef.docx");


    if(!file.open(QIODevice::ReadOnly))
    {
    }

    QByteArray content =  file.readAll();
    QString b64Content = content.toBase64();

    file.close();

    QFile file2("C:/Users/10694/Desktop/test/abcdef.docx");

    QByteArray byteArry(b64Content.toStdString().c_str());

    //qDebug()<<"\nbyteArry:\n"<<byteArry;

    QByteArray temp = byteArry.fromBase64(byteArry);
    byteArry = byteArry.fromBase64(byteArry);

    //qDebug()<<"\nmainDecodeArray:\n"<<byteArry;
    //qDebug()<<"\nmaintempArray:\n"<<temp;

    if(!file2.open(QIODevice::WriteOnly))
    {
    }

    file2.write(temp);
    file2.close();

*/




}
