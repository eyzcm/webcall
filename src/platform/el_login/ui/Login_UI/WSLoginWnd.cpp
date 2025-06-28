#include "WSLoginWnd.h"
#include "utils/Plat_EnumDefine.h"
#include "utils/UICommUtilsHelper.h"
#include "UILoginUserListWnd.h"
#include "control/LoginController.h"
#include "session/el_sessionservice.h"
#include <protocol/platprotocolservice.h>
#include "service/dbuiservice.h"

#include "protocol/usercenterhttpagent.h"

#include "protocol/serviceaddressmanager.h"

#include "utils/utilshelper.h"
#include <QDebug>
#include <QDesktopServices>
#include <QListWidget>
#include <QWidgetAction>

WSLoginWnd::WSLoginWnd(QWidget *parent, QString cmd) : QWidget(parent), m_menu(nullptr), m_pUserList(nullptr), m_isPopup(false)
{
	m_autoLogin = 0;
    ui.setupUi(this);
    m_jsHelper  = NULL;
    m_loginType = 0;
    /*
    this->setWindowFlags(windowFlags() & ~(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    this->setWindowModality(Qt::ApplicationModal);
*/
    m_ePresenceStatus = E_ONLINE;
    m_loginStatus     = E_LOGIN_NONE;

    ui.btn_login->setEnabled(false);
    this->installEventFilter(this);
    ui.btn_user_list->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    Init(cmd);
    CreateMenu();
    InitAccountList();

    ui.edit_user_name->installEventFilter(this);
    ui.edit_password->installEventFilter(this);
    ui.checkBox_save_pw->installEventFilter(this);
    ui.checkBox_auto_login->installEventFilter(this);
    ui.btn_user_status->installEventFilter(this);
    ui.btn_user_list->installEventFilter(this);

    connect(ui.btn_login, SIGNAL(clicked()), this, SLOT(DoLoginClick()));
    connect(ui.m_accountreg_btn, SIGNAL(clicked()), this, SLOT(DoRegClick()));
    connect(ui.edit_user_name, SIGNAL(textChanged(QString)), this, SLOT(R_UserNameTextChanged(QString)));
    connect(ui.edit_password, SIGNAL(textChanged(QString)), this, SLOT(R_PasswordTextChanged(QString)));
    connect(ui.checkBox_auto_login, SIGNAL(stateChanged(int)), this, SLOT(R_AutoLoginStateChanged(int)));
    connect(ui.checkBox_save_pw, SIGNAL(stateChanged(int)), this, SLOT(R_RememberStateChanged(int)));
    connect(gSessionService->LoginSignal(), SIGNAL(S_Relogin()), this, SLOT(ReDoLogin()));

    m_onHideTimer = new QTimer;
    connect(m_onHideTimer, SIGNAL(timeout()), this, SLOT(R_TimerHideAccountList()));

    //设置默认面板为账号登陆面板
    ui.stackedWidget->setCurrentIndex(0);

    QPalette pal = ui.webView_wx->palette();
    pal.setColor(QPalette::Background, QColor(0xff, 0xff, 0xff, 0xff));
    ui.webView_wx->setPalette(pal);

    /*pal = ui.ppwidget->palette();
    pal.setColor( QPalette::Background, QColor(0xff,0xff,0xff,0xff));
    ui.ppwidget->setPalette(pal);
    */
    ui.webView_wx->setAttribute(Qt::WA_TranslucentBackground, true);
    // ui.ppwidget->setAttribute(Qt::WA_TranslucentBackground,true);
    ui.webView_wx->setStyleSheet("background-color:transparent;");

    // ui.webView_wx->page()->(Qt::WA_TranslucentBackground,true);
    // ui.webView_wx->page()->setAttribute;

    m_jsHelper = new JsWxLoginHelper;
    m_jsHelper->SetWebView(ui.webView_wx, "wxObj");
    connect(m_jsHelper, SIGNAL(S_Login(QString, QString, QString)), this, SLOT(R_WXLogin(QString, QString, QString)));
    connect(m_jsHelper, SIGNAL(S_AuthorizationLoginDenied()), this, SLOT(R_AuthorizationLoginDenied()));
    connect(m_jsHelper, SIGNAL(S_BackAccountLogin()), this, SLOT(R_BackAccountLogin()));
    connect(m_jsHelper, SIGNAL(S_UpdateToken(QString)), this, SLOT(R_UpdateToken(QString)));
    connect(m_jsHelper, SIGNAL(S_JsLoginexception(int)), this, SLOT(R_JsLoginexception(int)));
    PlatProtocolService * server = PlatProtocolService::GetInstance();

    connect(gIProtocolLoginAgent , SIGNAL(S_AdminAuthLogin()), this, SLOT(R_AdminAuthLogin()));
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));

    //设置微信面板默认链接
#ifdef Q_OS_WIN
	
	QString pathDir = QString("%1/%2/%3").arg(UtilsHelper::GetRunAppDir()).arg("html").arg("accountlogin.html");
#else
    QString pathDir = QString("file:///%1/%2/%3").arg(UtilsHelper::GetRunAppDir()).arg("html").arg("accountlogin.html");
#endif // Q_OS_WIN

    

    ui.webView_wx->showMaximized();
    ui.webView_wx->setUrl(QUrl(pathDir));
    ui.webView_wx->page()->setBackgroundColor(Qt::transparent); //透明背景
}

WSLoginWnd::~WSLoginWnd()
{
}

//用户状态按钮响应槽函数
void WSLoginWnd::on_btn_user_status_clicked()
{
    //获取用户状态按钮坐标
    QPoint pos = ui.btn_user_status->pos() + ui.btn_user_status->parentWidget()->pos();
    pos        = this->mapToGlobal(pos);
    pos.setY(pos.y() + 20);

    //弹出菜单
    m_menu->exec(pos);
}

//用户列表下拉按钮
void WSLoginWnd::on_btn_user_list_clicked()
{
    if (!m_isPopup)
    {
        ShowUserListPanel();
    }
    else
    {
        HideUserListPanel();
    }
}

//菜单按钮在线响应槽函数
void WSLoginWnd::Slot_Menu_OnLine_Clicked()
{
    InitState(0);
    InitUserStatusStyle(0);
}

//菜单按钮忙碌响应槽函数
void WSLoginWnd::Slot_Menu_Busy_Clicked()
{
    InitState(1);
    InitUserStatusStyle(1);
}

//菜单按钮离开响应槽函数
void WSLoginWnd::Slot_Menu_Leave_Clicked()
{
    InitState(2);
    InitUserStatusStyle(2);
}

void WSLoginWnd::Init(QString cmd)
{
    do
    {
        if (DoArgumentsLogin())
        {
            break;
        }
        if (DoLocalAccountLogin(cmd))
        {
            break;
        }
        if (DoInitLogin())
        {
            break;
        }
    } while (false);
}

void WSLoginWnd::R_AutoLoginByArguments()
{
    bool        ret       = false;
    QStringList arguments = QCoreApplication::arguments();

    qDebug() << "Arguments : " << arguments;
    if (arguments.count() == 7)
    {
		
        QString userName = arguments[2];
        QString password = arguments[3];
        QString server   = arguments[4];
        QString port     = arguments[5];

        gSessionService->RetryLoginTag(true);

        ServiceAddressManager::GetInstance()->AddAddressInfo(server, port, false);
        ui.checkBox_save_pw->setChecked(true);
        ui.checkBox_auto_login->setChecked(false);
        ui.edit_user_name->setText(userName);
        ui.edit_password->setText(password);
        InitState(0);
        InitUserStatusStyle(0);

        UILoginData *pdata = gUIUserInfoManager->CreateLoginInfo();
        pdata->IsAutoLogin(false);
        pdata->IsRemember(true);
        pdata->Password(password);
        pdata->UserName(userName);
        CommInfo *info = gSessionService->LocalConfig();
        if (info)
        {
            pdata->UserState(info->GetValueByKey("state").toInt());
        }
        else
        {
            pdata->UserState(0);
        }
        LoginController::GetInstance()->DoLogin(*pdata);
        ret = true;
    }
    return ;
}


bool WSLoginWnd::DoArgumentsLogin()
{
	bool ret = false;
	QStringList arguments = QCoreApplication::arguments();


	if (arguments.count() >= 5) {
		QTimer::singleShot(2000, this, SLOT(R_AutoLoginByArguments()));
		m_autoLogin = 1;
		ret = true;
	}
	return ret;
}


bool WSLoginWnd::DoLocalAccountLogin(QString cmd)
{
    bool ret = false;
    do
    {
        CommInfo *info = gSessionService->LocalConfig();
        if (info)
        {
            ret = InitLoginWnd(info, true, cmd);
            break;
        }
    } while (false);
    return ret;
}

bool WSLoginWnd::DoInitLogin()
{
    ui.btn_login->setEnabled(false);
    ui.checkBox_save_pw->setChecked(false);
    ui.checkBox_auto_login->setChecked(false);
    ui.edit_user_name->setText("");
    ui.edit_password->setText("");
    InitState(0);
    InitUserStatusStyle(0);
    return true;
}

void WSLoginWnd::keyPressEvent(QKeyEvent *arg)
{
    if (arg->key() == Qt::Key_Return || arg->key() == Qt::Key_Enter)
    {
        if (m_isPopup)
        {
            m_pUserList->EnterCurrentItem();
        }
        else if (ui.btn_login->isEnabled())
        {
            DoLoginClick();
        }
    }
    else if (arg->key() == Qt::Key_Up)
    {
        if (m_isPopup)
        {
            m_pUserList->MovePreItem();
        }
    }
    else if (arg->key() == Qt::Key_Down)
    {
        if (m_isPopup)
        {
            m_pUserList->MoveNextItem();
        }
    }

    // if (arg->key()== Qt::Key_Return|| arg->key() == Qt::Key_Enter )
    //{
    //	if (ui.edit_password->hasFocus())
    //	{
    //		DoLoginClick();
    //	}
    //}
}

void WSLoginWnd::SetLoginStatus(ELOGINSTATUS loginStatus)
{
    m_loginStatus = loginStatus;
}

void WSLoginWnd::ReDoLogin()
{
    if (m_tokenUid.isEmpty())
    {
        DoUserPassLogin();
    }
    else
    {
        QString      loginurl = m_tokenServerIp;
        addressinfo *info     = ServiceAddressManager::GetInstance()->GetAddressInfo();
        if (info != nullptr)
        {
            loginurl = QString("%1:%2").arg(info->m_host).arg(info->m_port);
        }

        UILoginData *pdata = gUIUserInfoManager->CreateLoginInfo();

        pdata->IsAutoLogin(0);
        pdata->IsRemember(0);
        pdata->Password(m_tokenData);
        pdata->UserName(m_tokenUid);
        pdata->UserState((int)m_ePresenceStatus - 1);
        pdata->LoginType(1);

        LoginController::GetInstance()->WxDoLogin(*pdata, loginurl);
    }
}

void WSLoginWnd::DoRegClick()
{

    QString url = gSessionService->GetSystemConfigByKey("accountregurl", "https://easyliao.com/register.html");
    QDesktopServices::openUrl(QUrl(url));

}

void WSLoginWnd::DoLoginClick()
{
    m_tokenUid = "";
    DoUserPassLogin();
}

void WSLoginWnd::DoUserPassLogin()
{

    QString username = getUsername();
    QString password = getPassword();

    //过滤账号密码中的空格
    username = username.remove(QRegExp("\\s"));
    password = password.remove(QRegExp("\\s"));

    R_SetLoginBtnState(false);

    if (m_loginStatus == E_LOGIN_PRE || m_loginStatus == E_LOGIN_ERROR || m_loginStatus == E_LOGIN_NONE)
    {
        // do nothing.
    }
    else
    {
        return;
    }

    bool isRemember = false;
    if (ui.checkBox_save_pw->isChecked())
    {
        isRemember = true;
    }
    bool isAutoLogin = false;
    if (ui.checkBox_auto_login->isChecked())
    {
        if ((username.isEmpty()) || (password.isEmpty()))
        {
            ui.checkBox_auto_login->setChecked(false);
            return;
        }
        else
            isAutoLogin = true;
    }

    UILoginData *pdata = gUIUserInfoManager->CreateLoginInfo();
    pdata->IsAutoLogin(isAutoLogin);
    pdata->IsRemember(isRemember);
    pdata->Password(password);
    pdata->UserName(username);
    pdata->LoginType(0);
    pdata->UserState((int)m_ePresenceStatus - 1);
    gUIUserInfoManager->LoginData(pdata);
    LoginController::GetInstance()->DoLogin(*pdata);
}


QString WSLoginWnd::getUsername()
{
    //过滤账号密码中的空格
    QString username = ui.edit_user_name->text().trimmed();
    username         = username.remove(QRegExp("\\s"));
    return username;
}

QString WSLoginWnd::getPassword()
{
    QString password = ui.edit_password->text().trimmed();
    password         = password.remove(QRegExp("\\s"));
    return password;
}


void WSLoginWnd::R_AutoLoginStateChanged(int state)
{
    if (ui.checkBox_auto_login->isChecked())
    {
        ui.checkBox_save_pw->setChecked(true);
    }
}

void WSLoginWnd::R_RememberStateChanged(int state)
{
    if (!ui.checkBox_save_pw->isChecked())
    {
        ui.checkBox_auto_login->setChecked(false);
    }
}

void WSLoginWnd::R_OnDeleteAccount(QString userName)
{
    gSessionService->DelectAccount(userName);
}

void WSLoginWnd::R_OnSelectAccountClicked()
{
    QWidgetAction *action  = (QWidgetAction *)this->sender();
    auto           data    = action->data().toString();
    CommInfo      *account = gSessionService->GetAccountById(data);
    if (account)
    {
        InitLoginWnd(account, false);
    }
}
void WSLoginWnd::R_AutoLogin()
{
    // HideUserListPanel();

    CommInfo *info = gSessionService->LocalConfig();
    if (info)
    {

        UILoginData *pdata = gUIUserInfoManager->CreateLoginInfo();
        pdata->IsAutoLogin(true);
        pdata->IsRemember(true);
        pdata->Password(info->GetValueByKey("password"));
        pdata->UserName(info->GetValueByKey("username"));
        pdata->UserState(info->GetValueByKey("state").toInt());
        pdata->LoginType(0);
        gUIUserInfoManager->LoginData(pdata);
        LoginController::GetInstance()->DoLogin(*pdata);
    }
}

bool WSLoginWnd::InitLoginWnd(CommInfo *info, bool islastLoginAccount, QString strCmd)
{
    bool ret = false;
    R_RestoreUserListArrow();

    if (info)
    {
	
		ServiceAddressManager::GetInstance()->Init(info->GetValueByKey("username"));

        do
        {
            if (info->GetValueByKey("autologin") == "1")
            {

                ui.btn_login->setEnabled(true);
                ui.checkBox_save_pw->setChecked(true);
                ui.checkBox_auto_login->setChecked(true);
                ui.edit_user_name->setText(info->GetValueByKey("username"));
                ui.edit_password->setText(info->GetValueByKey("password"));

                int nState = info->GetValueByKey("state").toInt();
                InitState(nState);
                InitUserStatusStyle(nState);

                if ((strCmd != "logout") && (islastLoginAccount))
                {
                    R_SetLoginBtnState(false);
                    QTimer::singleShot(1000, this, SLOT(R_AutoLogin()));
                }

                ret = true;
                break;
            }

            if (info->GetValueByKey("autologin") == "0" && info->GetValueByKey("rememberpassword") == "1")
            {
                ui.btn_login->setEnabled(true);
                ui.checkBox_save_pw->setChecked(true);
                ui.checkBox_auto_login->setChecked(false);
                ui.edit_user_name->setText(info->GetValueByKey("username"));
                ui.edit_password->setText(info->GetValueByKey("password"));

                int nState = info->GetValueByKey("state").toInt();
                InitState(nState);
                InitUserStatusStyle(nState);

                ret = true;
                break;
            }

            ui.btn_login->setEnabled(false);
            ui.checkBox_save_pw->setChecked(false);
            ui.checkBox_auto_login->setChecked(false);
            ui.edit_user_name->setText(info->GetValueByKey("username"));
            ui.edit_password->setText("");
            InitState(0);
            InitUserStatusStyle(0);
            ret = true;
        } while (false);
	}
	else
	{
		ServiceAddressManager::GetInstance()->Init("");
	}
    return ret;
}

//曹振超 2019年9月17日
void WSLoginWnd::R_RestoreUserListArrow()
{
    if (m_isPopup)
    {
        HideUserListPanel();
    }
}

void WSLoginWnd::R_UserNameTextChanged(QString text)
{
    ui.edit_password->setText("");

    // CZC：弹出搜索列表
    m_pUserList->SetSearchKey(text);
    ShowUserListPanel();
}

void WSLoginWnd::R_PasswordTextChanged(QString password)
{
    QString strName = getUsername();
    if (!strName.isEmpty() && !password.isEmpty())
    {
        ui.btn_login->setEnabled(true);
    }
    else
    {
        ui.btn_login->setEnabled(false);
    }
}

void WSLoginWnd::CreateMenu()
{
    //创建用户状态菜单
    QAction *pActionOnLine = new QAction(QIcon(":/Resources/SVG/a_online_state.svg"), QString::fromLocal8Bit(tr("在线").toStdString().c_str()), this);
    QAction *pActionBusy   = new QAction(QIcon(":/Resources/SVG/a_busy_state.svg"), QString::fromLocal8Bit(tr("忙碌").toStdString().c_str()), this);
    QAction *pActionLeave  = new QAction(QIcon(":/Resources/SVG/a_leave_state.svg"), QString::fromLocal8Bit(tr("离开").toStdString().c_str()), this);

    connect(pActionOnLine, SIGNAL(triggered()), this, SLOT(Slot_Menu_OnLine_Clicked()));
    connect(pActionBusy, SIGNAL(triggered()), this, SLOT(Slot_Menu_Busy_Clicked()));
    connect(pActionLeave, SIGNAL(triggered()), this, SLOT(Slot_Menu_Leave_Clicked()));

    m_menu = new QMenu(this);
    m_menu->addAction(pActionOnLine);
    m_menu->addAction(pActionBusy);
    m_menu->addAction(pActionLeave);
    m_menu->hide();
}

void WSLoginWnd::InitState(int nState)
{
    if (nState == 0)
    {
        m_ePresenceStatus = E_ONLINE;
    }
    else if (nState == 1)
    {
        m_ePresenceStatus = E_BUSY;
    }
    else if (nState == 2)
    {
        m_ePresenceStatus = E_LEAVE;
    }
    else
    {
        m_ePresenceStatus = E_ONLINE;
    }
}

void WSLoginWnd::InitUserStatusStyle(int nState)
{
    static QString styleFormat = "QPushButton{border-image: url(:/Resources/SVG/%1.svg);}";
    QString        style       = "";
    if (nState == 0)
    {
        style = styleFormat.arg("a_online_state");
    }
    else if (nState == 1)
    {
        style = styleFormat.arg("a_busy_state");
    }
    else if (nState == 2)
    {
        style = styleFormat.arg("a_leave_state");
    }
    else
    {
        style = styleFormat.arg("a_online_state");
    }

    ui.btn_user_status->setStyleSheet(style);
}

void WSLoginWnd::InitAccountList()
{
    m_pUserList = new UILoginUserListWnd(this);
    m_pUserList->installEventFilter(this);
    m_pUserList->hide();
    connect(m_pUserList, SIGNAL(S_OnItemClick(CommInfo *)), this, SLOT(R_OnItemClick(CommInfo *)));
    connect(m_pUserList, SIGNAL(S_UnActive()), this, SLOT(R_RestoreUserListArrow()));
}

void WSLoginWnd::R_OnItemClick(CommInfo *account)
{
    if (account)
    {
		QString username = account->GetValueByKey("username");
		if (username.isEmpty() == false)
		{

		}
        InitLoginWnd(account, false);
        HideUserListPanel();
    }
}

void WSLoginWnd::R_SetLoginBtnState(bool normal)
{
    if (normal)
    {
        ui.btn_login->setText(tr("登录"));
        ui.btn_login->setEnabled(true);
        ui.btn_user_list->setEnabled(true);
        ui.edit_user_name->setEnabled(true);
        ui.edit_password->setEnabled(true);
        ui.checkBox_save_pw->setEnabled(true);
        ui.checkBox_auto_login->setEnabled(true);
    }
    else
    {
        ui.btn_login->setText(tr("登录中..."));
        ui.btn_login->setEnabled(false);
        ui.btn_user_list->setEnabled(false);
        ui.edit_user_name->setEnabled(false);
        ui.edit_password->setEnabled(false);
        ui.checkBox_save_pw->setEnabled(false);
        ui.checkBox_auto_login->setEnabled(false);
    }
}



void WSLoginWnd::R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
{
    if ((newStatus == E_LOGIN_OK_MAINTCP)||(newStatus == E_LOGIN_ALLOK))
	{

        if (oldStatus == E_LOGIN_DOING)
		{

			{
				static int inited = 0;
				if (inited == 0)
				{
					inited = 1;
					LoginController::GetInstance()->CloseLoginWnd();

				}
			}
		}
	}
	if (E_LOGIN_ERROR == newStatus)
	{
		if (oldStatus == E_LOGIN_DOING)
        {
			if (gSessionService->GetErrorMsg() == tr("登录失败，当前账号已被托管，请先取消云端托管！"))
			{
				if (UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("登录失败，当前账号已在云端托管，请先取消云端托管！"), tr("登录客户端"), tr("知道了")) == 1)
				{
					if (UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("点击【确定】按钮后，云端账号将强制下线，云端不会自动托管！"), tr("确定"), tr("取消")) == 1)
					{
						UILoginData *pdata = gUIUserInfoManager->CreateLoginInfo();
						pdata->IsAutoLogin(0);
						pdata->IsRemember(0);
						pdata->Password(m_tokenData);
						pdata->UserName(m_tokenUid);
						pdata->LoginType(2);
						pdata->UserState((int)m_ePresenceStatus - 1);

						LoginController::GetInstance()->WxDoLogin(*pdata, m_tokenServerIp);
                        return ;
					}
				}

			}
			else
			{
				UICommUtilsHelper::ShowCommonBox(tr("提示"), gSessionService->GetErrorMsg());

				
			}

            on_btn_user_clicked();
		}
		
	}
}

void WSLoginWnd::on_btn_wx_clicked()
{
    m_loginType = 0;
    ui.btn_other->setStyleSheet("QPushButton{	border-image: url(:/Resources/SVG/icon_phone_normal.svg);}QPushButton:hover,QPushButton:pressed{	border-image: url(:/Resources/SVG/icon_phone_hover.svg);}");
    HideUserListPanel();
    ui.stackedWidget->setCurrentIndex(1);
    m_jsHelper->SetInitData();
    m_jsHelper->SetLoginType(m_loginType);
}

void WSLoginWnd::on_btn_other_clicked()
{
    if (m_loginType == 1)
    {
        m_loginType = 0;
        ui.btn_other->setStyleSheet("QPushButton{	border-image: url(:/Resources/SVG/icon_phone_normal.svg);}QPushButton:hover,QPushButton:pressed{	border-image: url(:/Resources/SVG/icon_phone_hover.svg);}");
    }
    else
    {
        m_loginType = 1;
        ui.btn_other->setStyleSheet("QPushButton{	border-image: url(:/Resources/SVG/icon_wx_normal.svg);}QPushButton:hover,QPushButton:pressed{	border-image: url(:/Resources/SVG/icon_wx_hover.svg);}");
    }
    HideUserListPanel();
    ui.stackedWidget->setCurrentIndex(1);
    m_jsHelper->SetInitData();
    m_jsHelper->SetLoginType(m_loginType);
}

void WSLoginWnd::on_btn_phone_clicked()
{
    m_loginType = 1;
    ui.btn_other->setStyleSheet("QPushButton{	border-image: url(:/Resources/SVG/icon_wx_normal.svg);}QPushButton:hover,QPushButton:pressed{	border-image: url(:/Resources/SVG/icon_wx_hover.svg);}");
    HideUserListPanel();
    ui.stackedWidget->setCurrentIndex(1);
    m_jsHelper->SetInitData();
    m_jsHelper->SetLoginType(m_loginType);
}

void WSLoginWnd::on_btn_user_clicked()
{
    ui.stackedWidget->setCurrentIndex(0);
    m_jsHelper->HideWxLogin();
}

bool WSLoginWnd::eventFilter(QObject *obj, QEvent *e)
{
    //然后再判断控件的具体事件 (这里指获得焦点事件)
    if (e->type() == QEvent::FocusIn)
    {
        if ((obj != ui.btn_user_list) && (obj != this))
        {
            if (m_pUserList)
            {
                m_onHideTimer->stop();
                m_onHideTimer->start(10);
            }
        }
    }

    if (e->type() == QEvent::Enter)
    {
        if ((obj == this) || (obj == m_pUserList))
        {
            m_onHideTimer->stop();
        }
    }

    if (e->type() == QEvent::Leave)
    {
        if (obj == this)
        {
            m_onHideTimer->start(200);
        }
    }

    if (obj == this)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            m_onHideTimer->stop();
            if (m_pUserList)
            {
                m_onHideTimer->start(10);
            }
        }
    }

    return QWidget::eventFilter(obj, e);
}

void WSLoginWnd::R_TimerHideAccountList()
{
    m_onHideTimer->stop();
    if (m_pUserList)
    {
        HideUserListPanel();
    }
}

void WSLoginWnd::ShowUserListPanel()
{
    m_isPopup = true;

    QString strStyle = "QPushButton{border-image: url(:/Resources/SVG/btn_pull_up.svg);}";
    ui.btn_user_list->setStyleSheet(strStyle);

    QPoint pt = ui.widget_1_user->pos() + ui.widget_name->pos() + ui.line_user->pos();
    m_pUserList->setGeometry(pt.x() - 2, pt.y() + 73, 270, 166);
    m_pUserList->Show();

    // this->setFocus();
    ui.btn_user_list->repaint();
    ui.btn_user_list->setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void WSLoginWnd::HideUserListPanel()
{
    m_isPopup = false;

    QString strStyle = "QPushButton{border-image: url(:/Resources/SVG/btn_pull_down.svg);}";
    ui.btn_user_list->setStyleSheet(strStyle);

    m_pUserList->hide();
    // ui.edit_user_name->setFocus();
}



void WSLoginWnd::R_WXLogin(QString uid, QString token, QString serverip)
{
	
    gUserCenterHttpAgent->Init(gSessionService->GetSystemConfigByKey("accountloginurl", "https://auth.easyliao.com"), token);
	gUserCenterHttpAgent->GetMenuList();
    gUserCenterHttpAgent->GetDeptList();
    gUserCenterHttpAgent->GetUserList();

    UILoginData *pdata = gUIUserInfoManager->CreateLoginInfo();
    pdata->IsAutoLogin(0);
    pdata->IsRemember(0);
    pdata->Password(token);
    pdata->UserName(uid);
    pdata->LoginType(1);

    m_tokenUid      = uid;
    m_tokenData     = token;
    m_tokenServerIp = serverip;
    pdata->UserState((int)m_ePresenceStatus - 1);

    UILoginData *poldData = gUIUserInfoManager->LoginData();
    if ((poldData == nullptr) || (poldData->UserName() != uid))
    {
        UILoginData *pnewdata = new UILoginData(pdata);
        pnewdata->Password("");

        gUIUserInfoManager->LoginData(pnewdata);
    }

    LoginController::GetInstance()->WxDoLogin(*pdata, serverip);
	
}

void WSLoginWnd::R_AdminAuthLogin()
{
    R_SetLoginBtnState(true);
    HideUserListPanel();
    ui.stackedWidget->setCurrentIndex(1);
    m_jsHelper->SetAccountWayData(getUsername(), getPassword());
}

void WSLoginWnd::R_AuthorizationLoginDenied()
{
    ui.stackedWidget->setCurrentIndex(0);
}

void WSLoginWnd::R_BackAccountLogin()
{
    ui.stackedWidget->setCurrentIndex(0);
    m_jsHelper->HideWxLogin();
}

void WSLoginWnd::R_JsLoginexception(int i)
{
    if (i == 0)
    {
        on_btn_phone_clicked();
    }
    else if (i == 1)
    {
        on_btn_wx_clicked();
    }
    else
    {
        on_btn_user_clicked();
    }
}
QString WSLoginWnd::CheckUrl()
{
    if (m_jsHelper)
        return m_jsHelper->CheckUrl();
    return "";
}

int WSLoginWnd::IsPwdSafed()
{
    if (m_jsHelper)
        return m_jsHelper->IsPwdSafed();
    return 0;
}

int WSLoginWnd::IsCheck()
{
	if (m_autoLogin == 1) return 0;
    if (m_jsHelper)
        return m_jsHelper->IsCheck();
    return 0;
}

void WSLoginWnd::R_UpdateToken(QString token)
{
    m_tokenData = token;
}
