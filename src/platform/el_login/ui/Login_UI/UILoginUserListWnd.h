#ifndef UILoginUserListWnd_H
#define UILoginUserListWnd_H

#include <QEvent>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QWidget>

#include "data/CommInfo.h"
#include "ui_UILoginUserListWnd.h"

class UILoginUserListWnd : public QWidget
{
    Q_OBJECT

public:
    UILoginUserListWnd(QWidget *parent = 0);
    ~UILoginUserListWnd();

public:
    void Init();
    void SetSearchKey(QString key);
    void AddAccountItems();
    void AddAccountItem(CommInfo *info, int nIndex);
    void Show();

    void MoveNextItem();
    void MovePreItem();
    void EnterCurrentItem();

signals:
    void S_OnItemClick(CommInfo *info);

    //曹振超 2019年9月17日
    void S_UnActive();

private slots:
    //接收到子控件发送的删除行消息
    void Slot_DeleteRow(QString userName);
    void R_OnItemClicked(QListWidgetItem *item);

private:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::UILoginUserListWnd ui;
    QString                m_searchKey;
    QList<CommInfo *>      m_listUserInfo;
};

#endif // UILoginUserListWnd_H
