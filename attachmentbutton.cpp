#include "attachmentbutton.h"



AttachmentButton::AttachmentButton(QWidget *parent)
    : QPushButton(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_menu = new QMenu(this);
    m_actionOne = new QAction(this);
    //m_actionTwo = new QAction(this);
    //m_actionThree = new QAction(this);

    m_actionOne->setText("Save as");
    //m_actionTwo->setText("ActionTwo");
    //m_actionThree->setText("ActionThree");

    m_menu->addAction(m_actionOne);
    //m_menu->addAction(m_actionTwo);
    //m_menu->addAction(m_actionThree);

    connect(m_actionOne, SIGNAL(triggered()), this, SLOT(actionOneSlot()));
    //connect(m_actionTwo, SIGNAL(triggered()), this, SLOT(actionTwoSlot()));
    //connect(m_actionThree, SIGNAL(triggered()), this, SLOT(actionThreeSlot()));
}

AttachmentButton::~AttachmentButton()
{

}

void AttachmentButton::actionOneSlot()
{
    QString path =QFileDialog::getExistingDirectory(this);
    filePath = path+"/"+fileName;

    if(path!="")
    {
        emit getFile(fileName,filePath);
    }
    //qDebug() << "filePath:"<<filePath;
}

void AttachmentButton::contextMenuEvent(QContextMenuEvent *event)
{
    QPushButton::contextMenuEvent(event);
    m_menu->exec(QCursor::pos());
}

void AttachmentButton::setFileName(QString qFileName)
{
    fileName = qFileName;
}
