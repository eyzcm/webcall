#include "stdafx.h"
#include "UISideTabWidget.h"

UISideTabWidget::UISideTabWidget(QWidget *parent) : QWidget(parent)
{

    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    // 创建QTabWidget，并设置滚动按钮
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->tabBar()->setUsesScrollButtons(true); // 使用滚动按钮

    // 添加QTabWidget到主布局中
    m_mainLayout->addWidget(m_tabWidget);

    // 设置布局
    setLayout(m_mainLayout);

    // 加载样式表
   LOADCSSFILE("UIChatAreaWidget.css");
}

void UISideTabWidget::AddTab(QWidget *widget, const QString &label)
{
    m_tabWidget->addTab(widget, label);
}
