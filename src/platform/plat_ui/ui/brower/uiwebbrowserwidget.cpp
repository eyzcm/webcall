#include "stdafx.h"
#include "UIWebBrowserWidget.h"
#include "UIWebPageView.h"


UIWebBrowserWidget::UIWebBrowserWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化地址栏和新标签页按钮
    m_addressBar = new QLineEdit(this);
    m_newTabButton = new QPushButton("+", this);

    // 初始化标签页容器
    m_tabWidget = new UIWebTabWidget(this);

    // 主布局
    m_mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(m_addressBar);
    topLayout->addWidget(m_newTabButton);

    m_mainLayout->addLayout(topLayout);
    m_mainLayout->addWidget(m_tabWidget);

    setLayout(m_mainLayout);

    // 连接信号和槽
    connect(m_newTabButton, &QPushButton::clicked, this, &UIWebBrowserWidget::OnNewTabButtonClicked);
    connect(m_addressBar, &QLineEdit::returnPressed, this, &UIWebBrowserWidget::OnAddressBarReturnPressed);

    // 加载样式表
    LOADCSSFILE("UIWebBrowserWidget.css");
}

void UIWebBrowserWidget::OpenPage(const QString &url)
{
    UIWebPageView *webView = new UIWebPageView();
    webView->LoadUrl(url);
    AddTab(url, webView);
}

void UIWebBrowserWidget::AddTab(const QString &title, QWidget *content)
{
    m_tabWidget->AddTab(title, content);
}

void UIWebBrowserWidget::SetCustomContextMenu(QMenu *menu)
{
    m_tabWidget->SetCustomContextMenu(menu);
}

void UIWebBrowserWidget::OnNewTabButtonClicked()
{
    OpenPage("about:blank");
}

void UIWebBrowserWidget::OnAddressBarReturnPressed()
{
    QString url = m_addressBar->text();
    OpenPage(url);
}
