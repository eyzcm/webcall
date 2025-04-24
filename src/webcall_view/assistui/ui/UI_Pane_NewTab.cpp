#include "UI_Pane_NewTab.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"

#include "session/el_sessionservice.h"
#include "data/UIShowSettingInfo.h"


UI_Pane_NewTab::UI_Pane_NewTab(QWidget *parent) : QWidget(parent)
{
    m_urlTimer = nullptr;
    m_isModify = false;
    ui.setupUi(this);
    m_webView = new UI_WebView_ShieldBackSpace(this);
    ui.verticalLayout->addWidget(m_webView);
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowSettingChanged()), this, SLOT(R_ShowSettingChanged()));
    InitData();
}

UI_Pane_NewTab::~UI_Pane_NewTab()
{
    if (m_urlTimer)
        m_urlTimer->deleteLater();
}

void UI_Pane_NewTab::InitData()
{
    m_newTabObj = new JsPanelNewTabHelper;
    m_newTabObj->SetWebView(m_webView, "newTabObj");
    if (m_urlTimer == nullptr)
    {

        m_urlTimer = new QTimer;
        m_urlTimer->setInterval(2000);
        connect(m_urlTimer, SIGNAL(timeout()), this, SLOT(R_OnUrlTimeOut()));
        m_urlTimer->start();
    }
}

void UI_Pane_NewTab::resizeEvent(QResizeEvent *event)
{
}

void UI_Pane_NewTab::R_OnUrlTimeOut()
{
    if (m_isModify)
    {
        m_isModify              = false;
        UIShowSettingInfo *info = gUIPersonalConfigYLService->ShowSettingInfo();
        if (info)
        {
            if (m_webView)
            {
                if (info->EnableThirdPage() == 1)
                {
                    QString strUrl = info->ThirdUrl();
                    if (m_url != strUrl)
                    {
                        m_url = strUrl;
                        m_newTabObj->SetUrl(m_url);
                    }
                }
            }
        }
    }
}

void UI_Pane_NewTab::R_ShowSettingChanged()
{
    m_isModify = true;
}
