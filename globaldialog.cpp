#include "globaldialog.h"
#include "ui_globaldialog.h"

GlobalDialog::GlobalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalDialog)
{
    ui->setupUi(this);
}

void GlobalDialog::setContent(QString title, QString content, bool succeed)
{
    setWindowTitle(title);
    ui->content->setText(content);
    ui->succeed->setVisible(succeed);
    ui->fail->setVisible(!succeed);
    show();
}

GlobalDialog::~GlobalDialog()
{
    delete ui;
}

void GlobalDialog::on_pushButton_clicked()
{
    close();
}
