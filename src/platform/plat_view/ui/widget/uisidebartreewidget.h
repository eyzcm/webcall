#ifndef UISIDEBARTREEWIDGET_H
#define UISIDEBARTREEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QMap>

class UISidebarTreeWidget : public QWidget {
    Q_OBJECT

public:
    explicit UISidebarTreeWidget(QWidget *parent = nullptr);

    // 对外接口，动态添加按钮和对应的 widget
    void AddButtonWithWidget(const QString &buttonText, QWidget *widget, const std::function<void()> &callback = nullptr);



private:
    QStackedWidget *m_stackedWidget;
    QMap<QPushButton*, QWidget*> m_buttonWidgetMap;  // 用于管理按钮和Widget的对应关系
    QVBoxLayout *m_mainLayout;                      // 主布局，用于动态添加按钮
    QHBoxLayout *m_buttonLayout;

    void setupUi(); // 初始化UI布局
};

#endif // UISIDEBARTREEWIDGET_H
