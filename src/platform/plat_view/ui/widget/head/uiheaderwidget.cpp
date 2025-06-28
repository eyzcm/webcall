#include "stdafx.h"
#include "UIHeaderWidget.h"
#include <QHBoxLayout>

UIHeaderWidget::UIHeaderWidget(QWidget *parent)
    : QWidget(parent),
      m_avatarWidget(new UI_Widget_User_Panel(this)),
      m_toolbarWidget(new UIHeadToolbarWidget(this)),
      m_customFunctionWidget(new UIHeadCustomFunctionWidget(this))
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_avatarWidget);
    layout->addWidget(m_toolbarWidget);
    layout->addWidget(m_customFunctionWidget);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);  // 设置各组件之间的间距
    setLayout(layout);

    // 加载样式表
    LoadStylesheet();
}

UIHeaderWidget::~UIHeaderWidget()
{
    delete m_avatarWidget;
    delete m_toolbarWidget;
    delete m_customFunctionWidget;
}

void UIHeaderWidget::LoadStylesheet()
{
    LOADCSSFILE("UIHeaderWidget.css");
}
