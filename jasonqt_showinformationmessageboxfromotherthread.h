#ifndef JASONQT_SHOWINFORMATIONMESSAGEBOXFROMOTHERTHREAD_H
#define JASONQT_SHOWINFORMATIONMESSAGEBOXFROMOTHERTHREAD_H

#include <QObject>
#include"globaldialog.h"
#include <QMessageBox>
#include <QEventLoop>

class JasonQt_ShowInformationMessageBoxFromOtherThread: public QObject
{
    Q_OBJECT

private:
    const QString m_title;
    const QString m_message;

public:
    JasonQt_ShowInformationMessageBoxFromOtherThread(const QString &title, const QString &message);
    static void show(const QString &title, const QString &message);
    static bool succeed;
private:
    void readyShow(void);

private slots:
    void onShow(void);
};

#endif // JASONQT_SHOWINFORMATIONMESSAGEBOXFROMOTHERTHREAD_H
