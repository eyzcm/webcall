#ifndef WSLoginWnd_H
#define WSLoginWnd_H

#include "ui_WSLoginWnd.h"
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPushButton>
#include <QWidget>

#include "utils/Plat_EnumDefine.h"
#include "ui/login_ui/JsWxLoginHelper.h"
#include "ui/MyIconStyle.h"
#include "UILoginUserListWnd.h"
#include "Widget_Login_UserList.h"
#include "data/CommInfo.h"

class WSLoginWnd : public QWidget
{
    Q_OBJECT

public:
    WSLoginWnd(QWidget *parent = 0, QString cmd = "");
    ~WSLoginWnd();

    QString          CheckUrl();
    int              IsCheck();
    int              IsPwdSafed();
    JsWxLoginHelper *m_jsHelper;

protected:
    bool eventFilter(QObject *obj, QEvent *e);
    void keyPressEvent(QKeyEvent *arg);

signals:
    void S_Closed();
    void S_HideWndAndDelCenterWidget();

private slots:
    void R_TimerHideAccountList();
    void ReDoLogin();
    void DoLoginClick();
    void DoRegClick();

    void R_UpdateToken(QString token);
    //曹振超 2019年9月17日
    void R_RestoreUserListArrow();

    void R_UserNameTextChanged(QString text);
    void R_PasswordTextChanged(QString password);
    void R_AutoLoginStateChanged(int state);
    void R_RememberStateChanged(int state);
    void R_OnDeleteAccount(QString userName);
    void R_OnSelectAccountClicked();

public slots:

    void SetLoginStatus(ELOGINSTATUS loginStatus);
    void R_WXLogin(QString uid, QString token, QString serverip);
    void R_AdminAuthLogin();
    void R_AuthorizationLoginDenied();
    void R_BackAccountLogin();
    void R_JsLoginexception(int i);

    void R_AutoLogin();
    void on_btn_user_status_clicked(); //用户状态按钮响应槽函数
    void on_btn_user_list_clicked();   //用户列表下拉按钮
    void Slot_Menu_OnLine_Clicked();   //菜单按钮在线响应槽函数
    void InitUserStatusStyle(int nState);
    void Slot_Menu_Busy_Clicked();  //菜单按钮忙碌响应槽函数
    void Slot_Menu_Leave_Clicked(); //菜单按钮离开响应槽函数
    void R_OnItemClick(CommInfo *account);
    void R_SetLoginBtnState(bool normal);


	void R_AutoLoginByArguments();
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void on_btn_wx_clicked();
    void on_btn_user_clicked();
    void on_btn_phone_clicked();
    void on_btn_other_clicked();

private:
    void Init(QString cmd);
    void InitAccountList();
    void CreateMenu();
    bool InitLoginWnd(CommInfo *info, bool islastLoginAccount, QString strCmd = "");
    void InitState(int nState);

    bool DoArgumentsLogin();
    bool DoLocalAccountLogin(QString cmd);
    bool DoInitLogin();

    void ShowUserListPanel();
    void HideUserListPanel();
    void DoUserPassLogin();
    QString getUsername();
    QString getPassword();

private:
    Ui::WSLoginWndClass ui;
    UILoginUserListWnd *m_pUserList;
    bool                m_isPopup;
    EPRESENCESTATUS     m_ePresenceStatus;
    ELOGINSTATUS        m_loginStatus;
    QMenu              *m_menu;
    QTimer             *m_onHideTimer;

    QString m_tokenUid;
    QString m_tokenData;
    QString m_tokenServerIp;

    //    JsWxLoginHelper *m_jsHelper;

    int m_loginType; // 0 wx,1,phone
	int m_autoLogin;//0,非，1。重新登录， 
};

#endif // WSLoginWnd_H
