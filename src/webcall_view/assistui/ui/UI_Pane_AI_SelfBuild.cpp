#include "UI_Pane_AI_SelfBuild.h"
#include "control/CommonSignalService.h"
#include "control/geasyliaoframework.h"
#include "service/DbUiWebcallService.h"
#include "session/el_sessionservice.h"
#include "data/UIShowSettingInfo.h"


UI_Pane_AI_SelfBuild::UI_Pane_AI_SelfBuild(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    m_webView = new UI_WebView_ShieldBackSpace(this);
    ui.verticalLayout->addWidget(m_webView);
    m_jsobj = new JsPanelAIHelper;
    m_jsobj->SetWebView(m_webView, "yljsobj");
}

UI_Pane_AI_SelfBuild::~UI_Pane_AI_SelfBuild()
{
}

void UI_Pane_AI_SelfBuild::InitData(QString val)
{
    //加载页面
    m_jsobj->SetUrl(gEasyLiaoFramework->GetHtmlNameByKey("aiBuilder"));
    m_jsobj->SetInitData(val);
}

void UI_Pane_AI_SelfBuild::resizeEvent(QResizeEvent *event)
{
}
