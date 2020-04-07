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
}
