#ifndef UICONTENTWIDGET_H
#define UICONTENTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

#include "interface/uiyl_centercontentinterface.h"
class UIContentWidget : public UIYL_CenterContentInterface
{
    Q_OBJECT

public:
    explicit UIContentWidget(QWidget *parent = nullptr);

    void Init();
    // 添加一个新的 Tab
    void AddTab(const QString &tabName, QWidget *contentWidget);

    // 删除指定索引的 Tab
    void RemoveTab(int index);

    // 设置指定 Tab 为当前显示
    void SetCurrentTab(int index);

signals:
    void S_TabChanged(int index); // Tab 切换信号

private slots:
    void OnTabChanged(int index); // 处理 Tab 切换

private:
    QTabWidget *m_tabWidget; // Tab 控件
};

#endif // UICONTENTWIDGET_H
