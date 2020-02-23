#pragma once
#ifndef UPDATE_H
#define UPDATE_H
#include<QThread>

class Update : public QThread      //一个线程，用于获取邮箱里的所有信，用户点击更新时调用
{
    Q_OBJECT
public:
    QWidget *window;

protected:
    void run();

signals:
    void updateDone();      //更新完成信号
    void updateStart();      //更新开始信号
    void updateWrong();      //更新失败
};

#endif // UPDATE_H
