#include "mainwindow.h"

#include "database.h"
#include <QApplication>
#include "base64.h"
#include "globaldialog.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    qApp;
    MainWindow w;
    w.show();
    return a.exec();
}
