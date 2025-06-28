#include "stdafx.h"
#include "UIAvatarWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include "uiyl/common/UIStatusWidgetPanel.h"

UIAvatarWidget::UIAvatarWidget(QWidget *parent)
    : QWidget(parent),
      m_avatarImageLabel(new QLabel(this)),
      m_loginNameLabel(new QLabel(this)),
      m_statusComboBox(new UIStatusWidgetPanel(this))
{
    // 设置布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();

    // 初始化状态选项
    InitializeStatusOptions();

    // 头像和状态水平布局
    topLayout->addWidget(m_avatarImageLabel);
    topLayout->addWidget(m_statusComboBox);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_loginNameLabel);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);  // 设置各组件之间的间距
    setLayout(mainLayout);

    // 连接信号与槽


    // 加载样式表
    LoadStylesheet();
}

UIAvatarWidget::~UIAvatarWidget()
{
    delete m_avatarImageLabel;
    delete m_loginNameLabel;
    delete m_statusComboBox;
}

void UIAvatarWidget::SetAvatarImage(const QPixmap &pixmap)
{
    m_avatarImageLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void UIAvatarWidget::SetLoginName(const QString &name)
{
    m_loginNameLabel->setText(name);
}

void UIAvatarWidget::LoadStylesheet()
{
    LOADCSSFILE( "UIAvatarWidget.css");
}

void UIAvatarWidget::InitializeStatusOptions()
{

}
