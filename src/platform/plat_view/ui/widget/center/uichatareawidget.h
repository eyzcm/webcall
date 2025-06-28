#ifndef UICHATAREAWIDGET_H
#define UICHATAREAWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

class UIChatAreaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIChatAreaWidget(QWidget *parent = nullptr);

    // 添加聊天tab
    void AddChatTab(const QString &title, QWidget *widget);

    // 移除聊天tab
    void RemoveChatTab(int index);

private:
    QTabWidget *m_tabWidget;
    QVBoxLayout *m_layout;


};

#endif // UICHATAREAWIDGET_H
