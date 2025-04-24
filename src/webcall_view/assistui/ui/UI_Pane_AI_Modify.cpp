#include "UI_Pane_AI_Modify.h"
#include "control/CommonSignalService.h"
#include "control/geasyliaoframework.h"
#include "service/DbUiWebcallService.h"
#include "session/el_sessionservice.h"
#include "data/UIShowSettingInfo.h"


UI_Pane_AI_Modify::UI_Pane_AI_Modify(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    m_webView = new UI_WebView_ShieldBackSpace(this);
    ui.verticalLayout->addWidget(m_webView);
    m_jsobj = new JsPanelAIHelper;

    m_jsobj->SetWebView(m_webView, "yljsobj");
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ReturnAIData(QString, QString)), m_jsobj, SIGNAL(S_ReturnAIData(QString, QString)));
}

UI_Pane_AI_Modify::~UI_Pane_AI_Modify()
{
}

void UI_Pane_AI_Modify::InitData(QString val)
{
    m_jsobj->SetUrl(gEasyLiaoFramework->GetHtmlNameByKey("aiModify"));

    m_jsobj->SetInitData(val);
}

void UI_Pane_AI_Modify::resizeEvent(QResizeEvent *event)
{
}
