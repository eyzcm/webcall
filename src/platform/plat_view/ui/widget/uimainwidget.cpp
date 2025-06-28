#include "stdafx.h"
#include "UiMainWidget.h"
#include "./widget/head/UIHeaderWidget.h"
#include "./widget/UISidebarWidget.h"
#include "UIBottomWidget.h"
#include "./widget/uiimchatwidget.h"

UiMainWidget::UiMainWidget(QWidget *parent)
    : QWidget(parent),
      m_headerWidget(new UIHeaderWidget(this)),
      m_sidebarWidget(new UISidebarWidget(this)),
      m_stackedWidget(new QStackedWidget(this)),
      m_bottomWidget(new UIBottomWidget(this))
{
    m_headerWidget->setFixedHeight(80);
    m_sidebarWidget->setFixedWidth(40);
    m_bottomWidget->setFixedHeight(20);
    m_headerWidget->setStyleSheet("background-color:#00ffff");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QHBoxLayout *centralLayout = new QHBoxLayout();
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    UIImChatWidget * imwidget = new UIImChatWidget(this);
    m_stackedWidget->addWidget(imwidget );
    m_stackedWidget->setCurrentIndex(0);

    centralLayout->addWidget(m_sidebarWidget);
    centralLayout->addWidget(m_stackedWidget);

    mainLayout->addWidget(m_headerWidget);
    mainLayout->addLayout(centralLayout);
    mainLayout->addWidget(m_bottomWidget);

    setLayout(mainLayout);

    // 加载样式表
    LoadStylesheet();
}

UiMainWidget::~UiMainWidget()
{
    // Qt 自动管理子对象，无需手动释放内存
}

void UiMainWidget::LoadStylesheet()
{
    QFile file(":/stylesheets/UiMainWidget.css");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}
