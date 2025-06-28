#include "LoginController.h"
#include "utils/UICommUtilsHelper.h"
//#include "UIMainController.h"
//#include "control/IUIController.h"
//#include "control/geasyliaoframework.h"
#include "service/dbuiservice.h"
#include "protocol/RequestLogoff.h"
#include "protocol/RequestLogoutInfo.h"
#include "protocol/serviceaddressmanager.h"
#include "protocol/platprotocolservice.h"

#include "session/el_sessionservice.h"

#include "ui/baseWnd.h"
#include "ui/common/uiForceLogoutbox.h"

#include "ui/login_ui/UI_UserCheckWnd.h"
#include "ui/login_ui/UIYL_userCheckPanel.h"
#include "ui/uimessagebox.h"
//#include "uiservice/DbUiSystemConfigService.h"
#include "utils/utilshelper.h"
#include "service/DbUiUserInfoService.h"

#include <session/sessionservice.h>

#include <UI_LoginTransitionsPage.h>


LoginController::LoginController(void)
{

    m_loginwnd     = nullptr;
    m_baseloginwnd = nullptr;
    m_isShowCheck  = 0;
	m_timeout = nullptr;
    Init();
}

LoginController::~LoginController(void)
{

    if (m_loginwnd)
    {
        m_loginwnd->deleteLater();
    }
}

void LoginController::Init()
{

    DbUiService::GetInstance1()->AddService( DbUiUserInfoService::GetInstance() );

    el_sessionService::GetInstance()->AddService("el_sessionInterface",SessionService::GetInstance());
    el_sessionService::GetInstance()->AddService("LoginTransitionsPageInterface",UI_LoginTransitionsPage::GetInstance());

    connect(gSessionService->LoginSignal(), SIGNAL(S_ForceLogout()), this, SLOT(R_ForceLogout()));
    //connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));

    connect(gSessionService, SIGNAL(S_ShowAccountCheck( QWidget *   )), this, SLOT(R_ShowAccountCheck( QWidget *  )));
	



}

void LoginController::R_timeout()
{
	if (0)
	{
		static int init = 1;
		int i =  1;

		PlatProtocolService::GetInstance()->GetSessionService(i)->DisconnectTcp();
		PlatProtocolService::GetInstance()->GetMonitorTcpService(i)->DisconnectTcp();

	}

	if (0)
	{

		static int init = 1;
		int i =1;


		PlatProtocolService::GetInstance()->GetLoginAgentService(i)->InitTcpAgent("crm.easyliao.com", "1890");
		PlatProtocolService::GetInstance()->GetLoginAgentService(i)->DoExecLoginCmd("prd19zcm3", "1", 0, 0);
	}
}

void LoginController::DoLogin(UILoginData &data)
{
    if (CurrentLoginState() != E_LOGIN_DOING)
    {
        gSessionService->SetUserPass(data.UserName(), data.Password(), data.LoginType());
        gSessionService->Login(data.LoginType());
    }
}

void LoginController::WxDoLogin(UILoginData &data, QString loginurl)
{
    if (CurrentLoginState() != E_LOGIN_DOING)
    {
        QString     loginport   = "1890";
        QStringList channelList = loginurl.split(":");
        QString     loginserver = channelList[0];
        if (channelList.size() > 1)
            loginport = channelList[1];

        //ServiceAddressManager::GetInstance()->Init(gSessionService->LoginData());
        gIProtocolLoginAgent->InitTcpAgent(loginserver, loginport);

        // gUIUserInfoManager->LoginData(&data);
        // Sleep( 200);

        if (data.LoginType() == 0)
            gUIUserInfoManager->LoginData(&data);

        gSessionService->SetUserPass(data.UserName(), data.Password(), data.LoginType());
        gSessionService->Login(data.LoginType());
    }

#ifdef _DEBUG
	
#endif // DEBUG



}

ELOGINSTATUS LoginController::CurrentLoginState() const
{
    return gSessionService->Status();
}

void LoginController::CurrentLoginState(ELOGINSTATUS val)
{
    ; // gIProtocolAgent->CurrentLoginState( val );
}

void LoginController::ForceReLogin(int force)
{
}

void LoginController::R_ForceLogout()
{
    ForceLogout();
}

void LoginController::ForceLogout()
{
    QString realname = gUIUserInfoManager->UserInfo() ? gUIUserInfoManager->UserInfo()->RealName() : "";
    if (realname.length() > 20)
    {
        realname = realname.left(20) + "...";
    }
    QString strTip = tr("帐号：%1\n当前帐号已经在别处登录！\n请点击确认，退出客户端！").arg(realname);

  

    BaseWnd      *logoutbasewnd = new BaseWnd();
    UIForceLogoutBox *wnd           = new UIForceLogoutBox;
    connect(wnd, SIGNAL(S_ResultOK()), this, SLOT(R_QuitApplication()));
    wnd->SetMsgText(strTip);
    logoutbasewnd->AddContentWidget(wnd);

    logoutbasewnd->HideMaxBtn();
    logoutbasewnd->HideMinBtn();
    logoutbasewnd->HideCloseBtn();

    logoutbasewnd->SetTitleIconVisible(false);
    logoutbasewnd->SetTitleTextVisible(false);
    logoutbasewnd->SetDeleteOnClose(true);
    logoutbasewnd->SetModal();
    logoutbasewnd->ShowCenter();
    /*
    BaseMainWnd *bwnd = UIMainController::GetInstance()->BaseUIMainWnd();
    if (bwnd)
    {
        int index = QApplication::desktop()->screenNumber(bwnd);
        if (index < 0)
        {
            index = 0;
        }
        QScreen *sc = QGuiApplication::screens()[index];
        QRect    rc = sc->availableGeometry();
        int      x  = rc.width() / 2 - logoutbasewnd->width() / 2;
        int      y  = rc.height() / 2 - logoutbasewnd->height() / 2;
        logoutbasewnd->move(x + rc.x(), y + rc.y());
    }
    */

}

void LoginController::DoLogoutProtocol()
{
    RequestLogoutInfo req;
    QString           body = req.MakeXml();
    UtilsHelper::Execute(body);
}


void LoginController::ShowLoginWnd(QString strCmd)
{
	//ServiceAddressManager::GetInstance()->Init(gSessionService->LocalConfig()? gSessionService->LocalConfig()->GetValueByKey("username"):"");
    if (m_baseloginwnd == nullptr)
    {
        m_baseloginwnd = new BaseWnd;
        connect(m_baseloginwnd, SIGNAL(S_OnQuit()), this, SLOT(R_OnQuit()));
        m_baseloginwnd->InitSystemTrayIcon(false);
        if (m_loginwnd == nullptr)
        {
            m_loginwnd = new WSLoginWnd(nullptr, strCmd);
        }
        m_baseloginwnd->AddContentWidget(m_loginwnd);
        m_baseloginwnd->SetWndTitle(tr("登录"));
        QString iconUrl = ":/Resources/PNG/widget_main_bg.png";
        m_baseloginwnd->SetBackPanelIcon(iconUrl);
        m_baseloginwnd->HideMaxBtn();
        m_baseloginwnd->HideMinBtn();
        m_baseloginwnd->SetCommonButtonStyle("white", "close");
        m_baseloginwnd->SetTitleIconVisible(false);
        m_baseloginwnd->SetTitleTextVisible(false);
        m_baseloginwnd->SetDeleteOnClose(true);
        m_baseloginwnd->SetSystrayIconTooltip("Easyliao");
    }
    m_baseloginwnd->ShowCenter();
}

void LoginController::CloseLoginWnd()
{
    if (m_baseloginwnd)
    {
        m_loginwnd->R_SetLoginBtnState(true);
        m_baseloginwnd->hide();
        m_baseloginwnd->WndClear();
    }
}

void LoginController::SetLoginBtnState(bool normal)
{
    if (m_baseloginwnd)
    {
        m_loginwnd->R_SetLoginBtnState(normal);
        if (normal)
            m_loginwnd->on_btn_user_clicked();
    }
}

void LoginController::R_OnQuit()
{
    qApp->exit(0);
}

void LoginController::R_QuitApplication()
{

    DoLogoutProtocol();
    gSessionService->DoQuitApplication();



}

void LoginController::R_ShowAccountCheck( QWidget * wnd  )
{
    if (m_isShowCheck == 0)
    {
        m_isShowCheck = 1;
        if (m_loginwnd && (m_loginwnd->IsCheck() >= 1))
        {
            
            
            //BaseMainWnd *wnd = UIMainController::GetInstance()->BaseUIMainWnd();
            //QWidget * wnd = gSessionService->GetMainWnd();
            if (wnd)
            {
                wnd->setAttribute(Qt::WA_TranslucentBackground, true);
                UI_UserCheckWnd *panel = new UI_UserCheckWnd(wnd);
                panel->Init(m_loginwnd->IsCheck(), m_loginwnd->CheckUrl(), m_loginwnd->IsPwdSafed());
                if (wnd->size().width() > 500)
                {
                    panel->resize(wnd->size().width(), wnd->size().height());
                }
                panel->showNormal();
            }
            
        }
    }
}

