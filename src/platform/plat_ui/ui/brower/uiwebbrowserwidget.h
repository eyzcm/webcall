#ifndef UIWEBBROWSERWIDGET_H
#define UIWEBBROWSERWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include "UIWebTabWidget.h"

class UIWebBrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIWebBrowserWidget(QWidget *parent = nullptr);

    // 打开新页面
    void OpenPage(const QString &url);

    // 添加新的Tab
    void AddTab(const QString &title, QWidget *content);

    // 自定义右键菜单
    void SetCustomContextMenu(QMenu *menu);

private slots:
    void OnNewTabButtonClicked();
    void OnAddressBarReturnPressed();

private:
    UIWebTabWidget *m_tabWidget;
    QLineEdit *m_addressBar;
    QPushButton *m_newTabButton;
    QVBoxLayout *m_mainLayout;
};

#endif // UIWEBBROWSERWIDGET_H
