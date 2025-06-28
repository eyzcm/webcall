#include "stdafx.h"
#include <QTabBar>
#include "UIChatAreaWidget.h"

UIChatAreaWidget::UIChatAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    LOADCSSFILE("UIChatAreaWidget.css");
    m_tabWidget = new QTabWidget(this);
    m_layout = new QVBoxLayout(this);

    // 创建一个包含按钮的容器
       QWidget *buttonContainer = new QWidget(this);
       QHBoxLayout *layout = new QHBoxLayout(buttonContainer);
       layout->setContentsMargins(0, 0, 0, 0);

       QPushButton *btn1 = new QPushButton("Btn1", this);
       QPushButton *btn2 = new QPushButton("Btn2", this);


       layout->addWidget(btn1);
       layout->addWidget(btn2);
       layout->addStretch();  // 确保按钮靠右显示

       // 将按钮容器设置为右上角的小组件
       m_tabWidget->setCornerWidget(buttonContainer, Qt::TopRightCorner);


    m_layout->addWidget(m_tabWidget);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
}

void UIChatAreaWidget::AddChatTab(const QString &title, QWidget *widget)
{
    m_tabWidget->addTab(widget, title);
}

void UIChatAreaWidget::RemoveChatTab(int index)
{
    m_tabWidget->removeTab(index);
}
