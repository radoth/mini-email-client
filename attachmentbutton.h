#ifndef ATTACHMENTBUTTON_H
#define ATTACHMENTBUTTON_H

#include <QtWidgets/QPushButton>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QFileDialog>

class AttachmentButton: public QPushButton

{
    Q_OBJECT
public:
    AttachmentButton(QWidget *parent = 0);
    ~AttachmentButton();

    void setFileName(QString qFileName);

    void contextMenuEvent(QContextMenuEvent *event);


signals:
    void getFile (QString, QString); //自己的信号


private:
    QMenu *m_menu;
    QAction *m_actionOne;
    //QAction *m_actionTwo;
    //QAction *m_actionThree;
    QString fileName;
    QString filePath;

protected slots:
    void actionOneSlot();
    //void actionTwoSlot();
    //void actionThreeSlot();

};

#endif // ATTACHMENTBUTTON_H
