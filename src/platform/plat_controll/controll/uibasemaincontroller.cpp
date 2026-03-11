#include "uibasemaincontroller.h"

#include "controll/conversioncontroller.h"


#include "service/PlatCommonSignalService.h"
#include "service/platcommonmodelservice.h"
#include "qapplication.h"

#include "session/el_sessionservice.h"

#include "ui/uimessagebox.h"
#include "server/platuiservice.h"
#include "utils/ConstStringHelper.h"
#include "utils/EL_Util_Time.h"
#include "utils/UISingleton.h"
#include "utils/utilshelper.h"
#ifdef _MSC_VER
#include <windows.h>
#else
#define UINT unsigned int
#endif

UIBaseMainController::UIBaseMainController(void)
{
    m_baseuiMainWnd = nullptr;
    m_mainWnd       = nullptr;
    CreateGlobalAddAtom();
    m_isDisconnect        = 0;
    m_disconnectShowTimer = NULL;

    m_StateChangeTimer = new QTimer;
    connect(m_StateChangeTimer, SIGNAL(timeout()), this, SLOT(R_StateChangeTimer()));
    m_StateChangeTimer->setSingleShot(true);

    connect(gSessionService, SIGNAL(S_QuitApplication()), this, SLOT(R_QuitApplication()));
}

void UIBaseMainController::CreateGlobalAddAtom()
{
#ifdef _MSC_VER
    m_hotKeyScreenId      = GlobalAddAtom(L"easyliaoscreen");
    m_hotKeyShowMainWndId = GlobalAddAtom(L"easyliaoshowmainwnd");
#endif
}

UIBaseMainController::~UIBaseMainController(void)
{
    if (m_baseuiMainWnd)
    {
        gViewService->ClearBaseMainWnd(m_baseuiMainWnd);
    }
    if (m_mainWnd)
    {
        gViewService->ClearMainWnd(m_mainWnd);
        m_mainWnd = nullptr;
    }
}

void UIBaseMainController::Init()
{
}

IBaseMainWnd *UIBaseMainController::BaseUIMainWnd() const
{
    return m_baseuiMainWnd;
}

EasyLiaoMainWnd *UIBaseMainController::EasyLiaoMainUI() const
{
    return m_mainWnd;
}

void UIBaseMainController::ShowMainWnd()
{
    CreateBaseUIMainWnd();

    if (m_mainWnd)
    {
        InitConnectMainSignal(m_baseuiMainWnd);
    }
    else
    {
        CreateMainWnd();
        InitConnectMainSignal(m_baseuiMainWnd);
    }
//todo 9999
    //if( gUIPersonalConfigManager->Clienttype() != E_CLIENT_WEB )
        m_baseuiMainWnd->AddContentWidget(m_mainWnd, true);

    m_mainWnd->MainWnd(m_baseuiMainWnd);

    UIUserInfo *userInfo = gSessionService->UserInfo();
    if (userInfo)
    {
        m_currentUserId = userInfo->UserId();
    }

    m_baseuiMainWnd->ShowMaxWnd();
}

void UIBaseMainController::InitRegisterHotKey()
{

}

void UIBaseMainController::RegisterHotKeyByVal(QString strHotKey, int strAtomId)
{
#ifdef _MSC_VER
    //todo 9999
    if (!strHotKey.isEmpty())
    {
        strHotKey      = strHotKey.replace(" ", "");
        UINT vk        = GetKeyByShotKey(strHotKey);
        UINT modifyKey = GetModifyKeyByHotKey(strHotKey);
        RegisterHotKey((HWND) m_baseuiMainWnd->Widget()->winId(), strAtomId, modifyKey, vk); //注册热键
    }
#endif
}

unsigned int UIBaseMainController::GetModifyKeyByHotKey(QString strHotKey)
{

    QStringList hotKeyList = strHotKey.split("+");
    UINT        modifys    = 0;
 #ifdef _MSC_VER
    //todo 9999
    if (hotKeyList.indexOf("CTRL") != -1)
    {
        modifys = modifys | MOD_CONTROL;
    }
    if (hotKeyList.indexOf("ALT") != -1)
    {
        modifys = modifys | MOD_ALT;
    }

    if (hotKeyList.indexOf("SHIFT") != -1)
    {
        modifys = modifys | MOD_SHIFT;
    }
#endif
    return modifys;

}

unsigned int UIBaseMainController::GetKeyByShotKey(QString strHotKey)
{
    QString key = strHotKey.mid(strHotKey.lastIndexOf("+") + 1).trimmed();
    auto    val = key.toUcs4();
    UINT    ret = 0;
    if (val.size() > 0)
    {
        ret = val[0];
    }
    return ret;
}

void UIBaseMainController::CreateMainWnd()
{
    if (m_mainWnd == nullptr)
    {
        ET("UI_Widget_MainWindow");
        m_mainWnd = gViewService->CreateMainWnd();
    }
}

void UIBaseMainController::SetRobotState()
{
}

void UIBaseMainController::CloseMainWindow()
{
 #ifdef _MSC_VER
    if (m_baseuiMainWnd)
    {
        m_baseuiMainWnd->WndClear();
        UnregisterHotKey((HWND) m_baseuiMainWnd->Widget()->winId(), m_hotKeyScreenId);
        UnregisterHotKey((HWND) m_baseuiMainWnd->Widget()->winId(), m_hotKeyShowMainWndId);
    }
#endif
}

void UIBaseMainController::R_DoMainWndQuit()
{
    CloseMainWindow();
    qApp->exit(0);
}

void UIBaseMainController::R_DoStateChange(EPRESENCESTATUS fromstate, EPRESENCESTATUS tostate)
{
    if (fromstate == tostate)
    {
        //无效的状态切换,不需要任何操作
        return;
    }
    else if (m_StateChangeTimer->isActive())
    {
        PlatCommonSignalService::GetInstance()->DoShowToastMsg(tr("客服状态切换中..."), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
        return;
    }
    else
    {
        int re = 1;
        if ((tostate == E_BUSY || tostate == E_LEAVE) && fromstate == E_ONLINE)
        {
            UIMessageBox *wnd = new UIMessageBox;
            if (tostate == E_BUSY)
            {
                wnd->SetContentTip(tr("当前\"忙碌\"可能会影响到对话接待,确认忙碌吗?"));
            }
            else if (tostate == E_LEAVE)
            {
                wnd->SetContentTip(tr("当前\"离开\"可能会影响到对话接待,确认离开吗?"));
            }
            wnd->SetCancelBtnVisible(true);
            wnd->SetOkBtnText(tr("确认"));
            re = wnd->exec();
        }
        if (re == 1)
        {
            SetUserState(tostate); //通知TCP变更客服状态
            m_StateChangeTimer->setInterval(1000);
            m_StateChangeTimer->setProperty("fromstate", (int) fromstate);
            m_StateChangeTimer->start();
        }
    }
}

void UIBaseMainController::R_StateChangeTimer()
{
    if (m_StateChangeTimer->interval() == 1000)
    {
        PlatCommonSignalService::GetInstance()->DoSetState(E_WAITING);

        m_StateChangeTimer->setInterval(2000);
        m_StateChangeTimer->start();
    }
    else if (m_StateChangeTimer->interval() == 2000)
    {
        int fromstate = m_StateChangeTimer->property("fromstate").toInt();
        PlatCommonSignalService::GetInstance()->DoSetState((EPRESENCESTATUS) fromstate);
        UIMessageBox *wnd = new UIMessageBox;
        wnd->SetContentTip(tr("切换\"状态\"失败,请检查网络后重试"));
        wnd->SetCancelBtnVisible(true);
        wnd->SetOkBtnText(tr("确认"));
        wnd->exec();
    }
}

void UIBaseMainController::R_Received_KeepAlive_State(EPRESENCESTATUS state)
{
    if (m_baseuiMainWnd)
    {
        m_baseuiMainWnd->SetUserStateByKeepAlive(state);
    }
    m_StateChangeTimer->stop();
}

void UIBaseMainController::R_DoSetState(EPRESENCESTATUS state)
{
    if (m_baseuiMainWnd)
    {
        m_baseuiMainWnd->SetUserStateByKeepAlive(state);
    }
}



void UIBaseMainController::R_OnTimer()
{
    if (m_isDisconnect)
    {
        if (m_mainWnd)
        {
            m_mainWnd->SetDisconnectTip(m_isDisconnect);
        }
    }
}

void UIBaseMainController::R_OnSetMainWndVisible(bool isShow)
{
    if (m_baseuiMainWnd)
    {
        if (isShow)
        {
            m_baseuiMainWnd->Widget()->show();
        }
        else
        {
            m_baseuiMainWnd->Widget()->hide();
        }
    }
}

void UIBaseMainController::R_ShowSettingChanged()
{
#ifdef _MSC_VER
    UnregisterHotKey((HWND) m_baseuiMainWnd->Widget()->winId(), m_hotKeyScreenId);
    UnregisterHotKey((HWND) m_baseuiMainWnd->Widget()->winId(), m_hotKeyShowMainWndId);

    InitRegisterHotKey();
#endif
}

void UIBaseMainController::SetSkinImage(QString imgIcon)
{
    if (m_baseuiMainWnd)
    {
        m_baseuiMainWnd->SetBackPanelIcon(imgIcon);
    }
}

void UIBaseMainController::R_OnQuit()
{
    QString closeType = gSessionService->GetSystemConfigByKey(ConstStringHelper::m_cCloseMainWindow_Type, "1");

    int type = closeType.toInt();
    if (type == 2)
    {
        R_DoMainWndQuit();
    }
    else if (type == 3)
    {
        ShowQuitMessageTip();
    }
    else
    {
        ShowQuitMessageTip();
    }
}

void UIBaseMainController::ShowQuitMessageTip()
{
    UIMessageBox *wnd = new UIMessageBox;
    wnd->SetContentTip(tr("确认要退出账号吗？\n点击“确认”后，您将退出系统并离线！"));
    wnd->SetCancelBtnVisible(true);
    wnd->SetOkBtnText(tr("确认"));
    connect(wnd, SIGNAL(S_ResultOK(bool)), this, SLOT(R_ResultOK(bool)));
    wnd->exec();
}

void UIBaseMainController::R_ResultOK(bool isOk)
{
    if (isOk)
    {
        R_DoMainWndQuit();
    }
}

void UIBaseMainController::InitConnectMainSignal(IBaseMainWnd *wnd)
{
    if (m_mainWnd && wnd)
    {
        connect(m_mainWnd, SIGNAL(S_BubbleTipShowMessage(QString, QString)), wnd, SLOT(R_BubbleTipShowMessage(QString, QString)));
        connect(m_mainWnd, SIGNAL(S_OpenMainwnd()), wnd, SLOT(R_OnOpenMainWndWnd()));
        connect(m_mainWnd, SIGNAL(S_Closed()), wnd, SLOT(close()));

        CommInfo *info = gSessionService->LocalConfig();
        if (info)
        {
            QString closeType = gSessionService->GetSystemConfigByKey(ConstStringHelper::m_cCloseMainWindow_Type, "1");
            int     type      = closeType.toInt();
            wnd->SetCloseType(type);
        }
        else
        {
            wnd->SetCloseType(1);
        }
        connect(wnd, SIGNAL(S_OnQuit()), this, SLOT(R_OnQuit()));
        connect(wnd, SIGNAL(S_ActivateShortcut(const quint32, const quint32)), m_mainWnd, SLOT(R_ActivateShortcut(const quint32, const quint32)));
        connect(wnd, SIGNAL(S_ActivateShortcut(const quint32, const quint32)), PlatCommonSignalService::GetInstance(), SIGNAL(S_ActivateShortcut(const quint32, const quint32)));
    }
}

void UIBaseMainController::CreateBaseUIMainWnd()
{
    if (m_baseuiMainWnd == nullptr)
    {
        m_baseuiMainWnd = gViewService->CreateBaseMainWnd();
        m_baseuiMainWnd->SetTitle(true);
        connect(m_baseuiMainWnd, SIGNAL(S_SetPanelScale()), PlatCommonSignalService::GetInstance(), SIGNAL(S_SetPanelScale()));
        connect(m_baseuiMainWnd, SIGNAL(S_DoubleClickTrayIcon()), PlatCommonSignalService::GetInstance(), SIGNAL(S_DoubleClickTrayIcon()));
        InitRegisterHotKey();
        int             state         = gSessionService->LoginData()->UserState();
        EPRESENCESTATUS presenceState = E_ONLINE;

        if (state == 0)
        {
            presenceState = E_ONLINE;
        }
        else if (state == 1)
        {
            presenceState = E_BUSY;
        }
        else if (state == 2)
        {
            presenceState = E_LEAVE;
        }

        m_baseuiMainWnd->SetUserStateByKeepAlive(presenceState);

        //“易聊客服系统” + 登录姓名（登录昵称） + 登录ID。
        QString     title    = "";
        UIUserInfo *userInfo = gSessionService->UserInfo();
        if (userInfo)
        {
            title = tr("易聊客服系统 - ");
            title += userInfo->RealName();
            title += "(";
            title += userInfo->NickName();
            title += ") - ";
            title += userInfo->UserId();
        }
        else
        {
            title = gSessionService->GetSystemConfigByKey("maintitle", tr("Easy Liao智能在线客服系统"));
        }
        m_baseuiMainWnd->DoSetWindowTitle(title);
        m_baseuiMainWnd->SetDeleteOnClose(false);
        m_baseuiMainWnd->SetTitleIconVisible(false);
        m_baseuiMainWnd->SetSystrayIconTooltip(title);

        gSessionService->SetMainWnd(m_baseuiMainWnd->Widget());
    }
    m_baseuiMainWnd->SetDeleteOnClose(false);
}

void UIBaseMainController::SetWindowTitle(QString txtTitle)
{
    if (m_baseuiMainWnd)
    {
        m_baseuiMainWnd->SetWndTitle(txtTitle);
    }
}

void UIBaseMainController::R_DoubleClickTrayIcon()
{
    //主树定位到最近一条提醒上。
    // todo shizhenyu
}

void UIBaseMainController::R_StopTwinkleTip()
{
    if (m_baseuiMainWnd)
    {
        m_baseuiMainWnd->StopTwinkle();
    }
}

void UIBaseMainController::R_SetMainWndTop(bool isTopWnd)
{
#ifdef _MSC_VER
    //todo 9999
    if (m_baseuiMainWnd)
    {
        //置顶
        if (isTopWnd)
        {
            ::SetWindowPos(HWND(m_baseuiMainWnd->Widget()->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            m_baseuiMainWnd->setOnTopHint(true);
        }
        else
        {
            ::SetWindowPos(HWND(m_baseuiMainWnd->Widget()->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            m_baseuiMainWnd->setOnTopHint(false);
        }
    }
#endif
}


void UIBaseMainController::R_QuitApplication()
{
    CloseMainWindow();
    qApp->exit(0);
}


void UIBaseMainController::SetUserState(EPRESENCESTATUS state)
{
    //todo 9999
        //gIProtocolAgent->SetUserState(state); //通知TCP
}

