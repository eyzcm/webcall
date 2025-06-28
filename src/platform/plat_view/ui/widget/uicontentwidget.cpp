#include "UIContentWidget.h"

UIContentWidget::UIContentWidget(QWidget *parent)
    : UIYL_CenterContentInterface(parent), m_tabWidget(new QTabWidget(this))
{
    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_tabWidget);
    setLayout(layout);

    // 连接信号槽
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &UIContentWidget::OnTabChanged);
    Init();
}

void UIContentWidget::Init()
{
    //todo 9999
    //connect(this, SIGNAL(S_CenterTabChanged(E_YL_CENTER_TAB, MChatItemData *)), CommonSignalService::GetInstance(), SIGNAL(S_CenterTabChanged(E_YL_CENTER_TAB, MChatItemData *)));

    //todo 9999
    /*
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
    connect(gUIPersonalConfigManager->PersonalConfigSign(), SIGNAL(S_GetIconFinshed()), this, SLOT(R_GetIconFinshed()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_GetViewEnv()), this, SLOT(R_GetIconFinshed()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ExpirationReminder(QString)), this, SLOT(R_ExpirationReminder(QString)));

    connect(CommonSignalService::GetInstance(), SIGNAL(S_GetViewEnv()), this, SLOT(R_GetIconFinshed()));
    */


    // UIFontModel::GetInstance()->InitData();

    m_tabWidget->setCurrentIndex(0);


    //暂时保留原来新医疗版tab改变隐藏方式
    //todo 9999
    /*
        connect(CommonSignalService::GetInstance(), SIGNAL(S_OnYLMainTreeSelectTabChanged(E_YL_TREE_TAB, E_YL_TREE_TAB)), this, SLOT(R_MainTreeSelectTabChanged(E_YL_TREE_TAB, E_YL_TREE_TAB)));
        connect(CommonSignalService::GetInstance(), SIGNAL(S_OnSelectedChatItemData(MChatItemData *)), this, SLOT(R_OnSelectedChatItemData2(MChatItemData *)));
        */

}

void UIContentWidget::AddTab(const QString &tabName, QWidget *contentWidget)
{
    m_tabWidget->addTab(contentWidget, tabName);
}

void UIContentWidget::RemoveTab(int index)
{
    if (index >= 0 && index < m_tabWidget->count()) {
        QWidget *widget = m_tabWidget->widget(index);
        m_tabWidget->removeTab(index);
        delete widget; // 删除对应的内容 Widget
    }
}

void UIContentWidget::SetCurrentTab(int index)
{
    if (index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(index);
    }
}

void UIContentWidget::OnTabChanged(int index)
{
    emit S_TabChanged(index); // 发出 Tab 切换信号
}
