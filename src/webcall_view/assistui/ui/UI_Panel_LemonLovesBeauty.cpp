#include "UI_Panel_LemonLovesBeauty.h"
#include "control/CommonSignalService.h"
#include "control/geasyliaoframework.h"
#include "model/YL_Model/UIYL_ChatRecordModel.h"
#include "session/el_sessionservice.h"

UI_Panel_LemonLovesBeauty::UI_Panel_LemonLovesBeauty(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
	m_lemonLovesBeautyJsObj = nullptr;
    m_webView = new UI_WebView_ShieldBackSpace(this);
    ui.verticalLayout->addWidget(m_webView);

    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_GetViewEnv()), this, SLOT(R_GetViewEnv()));
}

UI_Panel_LemonLovesBeauty::~UI_Panel_LemonLovesBeauty()
{
    if (m_lemonLovesBeautyJsObj != nullptr)
    {
        m_lemonLovesBeautyJsObj->deleteLater();
        m_lemonLovesBeautyJsObj = nullptr;
    }
}

void UI_Panel_LemonLovesBeauty::InitData()
{
    m_lemonLovesBeautyJsObj = new YLJS_LemonLovesBeauty_RightPanel_Helper;
    m_lemonLovesBeautyJsObj->SetWebView(m_webView, "projectListObj");
    connect(m_lemonLovesBeautyJsObj, SIGNAL(S_ProjectLinks_Checked(QString, QString)), this, SLOT(R_ProjectLinks_Checked(QString, QString)));

    if (m_lemonLovesBeautyJsObj)
    {
        m_lemonLovesBeautyJsObj->SetInitData();
    }
}

void UI_Panel_LemonLovesBeauty::R_ProjectLinks_Checked(QString databody, QString links)
{
    UIYL_ChatRecordModel::GetInstance()->SendLemonMsg(databody, links, E_MSGTYPE_LEMON_PRODUCT);
}

void UI_Panel_LemonLovesBeauty::resizeEvent(QResizeEvent* event)
{
    if (m_lemonLovesBeautyJsObj)
    {
        m_lemonLovesBeautyJsObj->SizeChanged(event->size().width(), event->size().height());
    }
}

void UI_Panel_LemonLovesBeauty::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
{
    switch (newStatus)
    {
        case E_LOGIN_NONE:
            break;
        case E_LOGIN_PRE:
            break;
        case E_LOGIN_DOING:
            break;

        case E_LOGIN_OK_MAINTCP:
            if (oldStatus == E_LOGIN_DOING)
            {
                InitData();
            }
            break;
        case E_LOGIN_PRE_MONITORTCP:
            break;
        case E_LOGIN_ERROR:
            break;
        case E_LOGIN_ERROR_MAINTCP:
        case E_LOGIN_RETRY_MAINTCP:
            break;
        case E_LOGIN_ERROR_MONITORTCP:
        case E_LOGIN_RETRY_MONITORTCP:
            break;
        case E_LOGIN_ALLOK:
            break;
        case E_LOGIN_LOGOUT:

            break;
    }
}

void UI_Panel_LemonLovesBeauty::R_GetViewEnv()
{
    if (m_lemonLovesBeautyJsObj)
    {
        m_lemonLovesBeautyJsObj->SetUrl(gEasyLiaoFramework->GetHtmlNameByKey("LemonLovesBeauty_ProjectList"));
    }
}
