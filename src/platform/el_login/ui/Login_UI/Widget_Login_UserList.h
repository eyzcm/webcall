#pragma once

#include "ui_Widget_Login_UserList.h"
#include <QWidget>

class Widget_Login_UserList : public QWidget
{
    Q_OBJECT

public:
    Widget_Login_UserList(QWidget *parent = 0);
    ~Widget_Login_UserList();

public:
    void    SetContent(QString user, QString id, int index); //设置用户昵称、ID、再listWidget中的index
    QString GetUserName();
public slots:
    void Slot_RemoveCurItem(); //删除当前行

signals:
    void Signal_DeleteRow(QString username); //通知父窗口，删除位置为index的行

private:
    Ui::Widget_Login_UserListClass ui;
    int                            m_nIndex;
    QString                        m_userName;
};
