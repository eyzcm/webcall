#ifndef UISIDEBARWIDGET_H
#define UISIDEBARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include "ui/uivboxlayout.h"

class UISidebarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UISidebarWidget(QWidget *parent = nullptr);

    // 动态添加图标按钮，并指定点击回调函数
    void AddToolButton(const QString &iconPath, const QString &tooltip, std::function<void()> onClickCallback);

    // 删除指定的图标按钮
    void RemoveToolButton(int index);

signals:
    void S_ToolButtonClicked(int index); // 动态图标按钮点击信号

private slots:
    void OnMoreButtonClicked(); // 处理更多按钮点击

private:
    UIVBoxLayout *m_layout;            // 垂直布局
    QWidget *m_dynamicButtonContainer; // 动态按钮容器
    QPushButton *m_moreButton;        // 更多按钮
    QPushButton *m_exitButton;        // 退出按钮
    QMenu *m_moreMenu;                // 悬浮菜单

    void UpdateMoreButtonVisibility();        // 更新更多按钮
};

#endif // UISIDEBARWIDGET_H
