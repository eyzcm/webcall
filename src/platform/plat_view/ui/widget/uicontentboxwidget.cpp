#include "stdafx.h"
#include "UIContentBoxWidget.h"

UIContentBoxWidget::UIContentBoxWidget(QWidget *parent) : QWidget(parent)
{
    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    // 创建QStackedWidget
    m_stackedWidget = new QStackedWidget(this);

    // 添加QStackedWidget到主布局中
    m_mainLayout->addWidget(m_stackedWidget);

    // 设置布局
    setLayout(m_mainLayout);

    // 加载样式表
    LOADCSSFILE("UIContentBoxWidget.css");

}

void UIContentBoxWidget::AddWidget(QWidget *widget)
{
    m_stackedWidget->addWidget(widget);
}

void UIContentBoxWidget::SetCurrentIndex(int index)
{
    m_stackedWidget->setCurrentIndex(index);
}
