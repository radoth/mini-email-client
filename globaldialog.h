#ifndef GLOBALDIALOG_H
#define GLOBALDIALOG_H

#include <QDialog>

namespace Ui {
class GlobalDialog;
}

class GlobalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlobalDialog(QWidget *parent = nullptr);
    void setContent(QString title,QString content,bool succeed);
    ~GlobalDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::GlobalDialog *ui;
};

#endif // GLOBALDIALOG_H
