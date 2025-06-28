#include "Widget_Login_UserList.h"

Widget_Login_UserList::Widget_Login_UserList(QWidget *parent) : QWidget(parent), m_nIndex(-1)
{

    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Slot_RemoveCurItem()));
}

Widget_Login_UserList::~Widget_Login_UserList()
{
}

//设置用户昵称和ID
void Widget_Login_UserList::SetContent(QString user, QString id, int index)
{
    ui.label_user->setText(user);
    ui.label_id->setText(id);
    m_nIndex   = index;
    m_userName = id;
}

//删除当前行
void Widget_Login_UserList::Slot_RemoveCurItem()
{
    Q_EMIT Signal_DeleteRow(m_userName);
}

QString Widget_Login_UserList::GetUserName()
{
    return m_userName;
}
