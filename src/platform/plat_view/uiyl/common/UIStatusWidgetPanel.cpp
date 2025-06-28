#include "stdafx.h"
#include "UIStatusWidgetPanel.h"

#include "ui/MyIconStyle.h"
#include "session/el_sessionservice.h"
#include "ui/uimessagebox.h"
#include "utils/ConstStringHelper.h"
#include "utils/utilshelper.h"
#include <QProcess>
#include "ui_UIStatusWidgetPanel.h"


UIStatusWidgetPanel::UIStatusWidgetPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::UIStatusWidgetPanel)
{
    ui->setupUi(this);

    //todo 9999
    /*connect(this, SIGNAL(S_DoQuit()), CommonSignalService::GetInstance(), SIGNAL(S_DoQuit()));

    connect(CommonSignalService::GetInstance(), SIGNAL(S_DoSetState(EPRESENCESTATUS)), this, SLOT(R_Received_KeepAlive_State(EPRESENCESTATUS)));  //状态变化时中间状态,切换中,超时变更回原来的状态
    connect(gIProtocolAgent, SIGNAL(S_Received_KeepAlive_State(EPRESENCESTATUS)), this, SLOT(R_Received_KeepAlive_State(EPRESENCESTATUS))); //TCP通知 确认客户端状态
*/
    QString style = "QToolButton{border:none;color: #FFFFFF;background-color: rgba(244, 244, 244, 0);} "
                    "QToolButton:hover{border:none;color: #FFFFFF;background-color: rgba(244, 244, 244, 0);} "
                    "QToolButton:pressed{border:none;color: #FFFFFF;background-color: rgba(244, 244, 244, 0);}";
    ui->m_stateBtn->SetMainBtnStyle(style);
    ui->m_stateBtn->SetMenuButtonStyle("background-color: rgba(244, 244, 244, 0);");
    ui->m_stateBtn->SetArrawBtnVisible(false);
    connect(ui->m_stateBtn, SIGNAL(S_MainBtnClicked()), this, SLOT(R_OnUserStateBtnClick()));
    connect(ui->m_stateBtn, SIGNAL(S_ArrawBtnClicked()), this, SLOT(R_OnUserStateBtnClick()));
    connect(ui->m_status_sel, SIGNAL(clicked()), this, SLOT(R_OnUserStateBtnClick()));
    connect(ui->m_status_text, SIGNAL(clicked()), this, SLOT(R_OnUserStateBtnClick()));
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));

    ui->m_stateBtn->SetMainBtn(":/Resources/SVG/a_online_state.svg", "");
}

UIStatusWidgetPanel::~UIStatusWidgetPanel()
{
}

void UIStatusWidgetPanel::CreateAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data, int flag)
{
    {
        QAction *action = new QAction(QIcon(iconUri), text, this);
        connect(action, SIGNAL(triggered()), this, SLOT(R_OnStateMenuClicked()));
        action->setData(QVariant(data));
        menu->addAction(action);
    }
    if (flag == 1)
    {
        QAction *action = new QAction(menu);
        action->setSeparator(true);
        menu->addAction(action);
    }
}

void UIStatusWidgetPanel::R_OnStateMenuClicked()
{
    QAction *action = (QAction *) this->sender();
    QString  data   = action->data().toString();

    if (StringCompare(ConstStringHelper::m_cQuit, data))
    {
        DoQuit();
    }
    else if (data == ConstStringHelper::m_cLogout)
    {
        DoLogout();
    }
    else if (data == ConstStringHelper::m_cCloudlogin)
    {
        DoCloudlogin();
    }
    else
    {
        DoSetState(data);
    }
}

void UIStatusWidgetPanel::DoQuit()
{
    UIMessageBox *box = new UIMessageBox;
    box->setWindowTitle(tr("提示"));
    box->SetContentTip(tr("确定退出当前系统。"));
    connect(box, SIGNAL(S_ResultOK(bool)), this, SLOT(R_ResultOK(bool)));
    box->exec();
}

void UIStatusWidgetPanel::DoLogout()
{
    UIMessageBox *box = new UIMessageBox;
    box->setWindowTitle(tr("提示"));
    box->SetContentTip(tr("确定返回登录界面？"));
    connect(box, SIGNAL(S_ResultOK(bool)), this, SLOT(R_ResultOK_Logout(bool)));
    box->exec();
}

void UIStatusWidgetPanel::DoCloudlogin()
{
    //todo 9999
    /*
    EL_Common_Popup_UI *box = new EL_Common_Popup_UI;
    box->Cloudlogin();
    */
}

void UIStatusWidgetPanel::DoSetState(QString data)
{
    EPRESENCESTATUS tostate = UtilsHelper::GetPresenceStatusByString(data);
    EPRESENCESTATUS fromstate = E_ONLINE;
    if(ui->m_status_text->text() == tr("在线"))
    {
        fromstate = E_ONLINE;
    }
    else if(ui->m_status_text->text() == tr("忙碌"))
    {
        fromstate = E_BUSY;
    }
    else if(ui->m_status_text->text() == tr("离开"))
    {
        fromstate = E_LEAVE;
    }
    //todo 9999
    //CommonSignalService::GetInstance()->DoStateChange(fromstate, tostate);
}

void UIStatusWidgetPanel::R_ResultOK(bool isOK)
{
    if (isOK)
    {
        //todo 9999
        //gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_47");
        Q_EMIT S_DoQuit();
    }
}

void UIStatusWidgetPanel::R_ResultOK_Logout(bool isOK)
{
    if (isOK)
    {
        //todo 9999
        //gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_46");
        gSessionService->DoLogoffProtocol();
    }
}

void UIStatusWidgetPanel::R_ProcessCallStarted()
{
    Q_EMIT S_DoQuit();
}

void UIStatusWidgetPanel::R_OnUserStateBtnClick()
{
    ui->m_stateBtn->ShowMenu();
}

void UIStatusWidgetPanel::InitState()
{
    int state = gSessionService->LoginData()->UserState();
    if (state == 0)
    {
        ui->m_stateBtn->SetMainBtn(":/YL/Resources/YL/UserInfo/UserState_Menu_Online.png", "");
        ui->m_status_text->setText(tr("在线"));
    }
    else if (state == 1)
    {
        ui->m_stateBtn->SetMainBtn(":/YL/Resources/YL/UserInfo/UserState_Menu_Busy.png", "");
        ui->m_status_text->setText(tr("忙碌"));
    }
    else if (state == 2)
    {
        ui->m_stateBtn->SetMainBtn(":/YL/Resources/YL/UserInfo/UserState_Menu_Leave.png", "");
        ui->m_status_text->setText(tr("离开"));
    }
    else
    {
        ui->m_stateBtn->SetMainBtn(":/YL/Resources/YL/UserInfo/UserState_Menu_Online.png", "");
        ui->m_status_text->setText(tr("在线"));
    }
    QMenu *statemenu = new QMenu();
    CreateAction(statemenu, ":/Resources/SVG/a_online_state.svg", tr("在线"), ConstStringHelper::m_cOnline, 1);
    CreateAction(statemenu, ":/Resources/SVG/a_busy_state.svg", tr("忙碌"), ConstStringHelper::m_cBusy, 1);
    CreateAction(statemenu, ":/Resources/SVG/a_leave_state.svg", tr("离开"), ConstStringHelper::m_cLeave, 1);
    CreateAction(statemenu, ":/Resources/SVG/logoff.svg", "注销", ConstStringHelper::m_cLogout, 1);
    if (gSessionService->HasPermissionByKey("loginTrust"))
    {
        CreateAction(statemenu, ":/Resources/SVG/cloudlogin.svg", tr("云端登录"), ConstStringHelper::m_cCloudlogin, 1);
    }
    CreateAction(statemenu, ":/Resources/SVG/quit.svg", tr("退出"), ConstStringHelper::m_cQuit);
    ui->m_stateBtn->SetMenu(statemenu);
}

void UIStatusWidgetPanel::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
{
    switch (newStatus)
    {
        case E_LOGIN_OK_MAINTCP:
            if (oldStatus == E_LOGIN_DOING)
            {
                InitState();
            }
            break;
    }
}

void UIStatusWidgetPanel::R_Received_KeepAlive_State(EPRESENCESTATUS state)
{
    switch (state)
    {
        case E_ONLINE:
            {
                ui->m_stateBtn->SetMainBtn(":/YL/Resources/YL/UserInfo/UserState_Menu_Online.png", "");
                ui->m_status_text->setText(tr("在线"));
            }
            break;
        case E_BUSY:
            {
                ui->m_stateBtn->SetMainBtn(":/YL/Resources/YL/UserInfo/UserState_Menu_Busy.png", "");
                ui->m_status_text->setText(tr("忙碌"));
            }
            break;
        case E_LEAVE:
            {
                ui->m_stateBtn->SetMainBtn(":/YL/Resources/YL/UserInfo/UserState_Menu_Leave.png", "");
                ui->m_status_text->setText(tr("离开"));
            }
            break;
        case E_WAITING:
            {
                ui->m_stateBtn->SetMainBtn(":/Resources/SVG/a_switching_state.svg", "");
                ui->m_status_text->setText(tr("切换中"));
            }
            break;
    }
}
