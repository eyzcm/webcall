#pragma once
#include "utils/Plat_EnumDefine.h"
#include "utils/UISingleton.h"
#include "data/UILoginData.h"
#include "ui/Login_UI/WSLoginWnd.h"
#include "ui/BaseWnd.h"
#include "data/UIVisitorUploadDataInfo.h"
#include <QObject>


class LoginController : public QObject, public UISingleton<LoginController>
{
    Q_OBJECT
    friend class UISingleton<LoginController>; //因为基类在getInstance()中 new T() 时会调用到A的构造函数

private:
    LoginController(void);
    virtual ~LoginController(void);

public:
	BaseWnd *getBaseMainWnd()
      {
          return m_baseloginwnd;
      }
    UIVisitorUploadDataInfo *CreateUploadData();
    void         Init();
    void         DoLogin(UILoginData &data);
    void         WxDoLogin(UILoginData &data, QString server);
    ELOGINSTATUS CurrentLoginState() const;
    void         CurrentLoginState(ELOGINSTATUS val);
    void         ForceLogout();
    void         ForceReLogin(int force);
    void         ShowLoginWnd(QString strCmd);
    void         CloseLoginWnd();
    void         SetLoginBtnState(bool normal);

    void DoLogoutProtocol();

    
    void HideWxLogin();


private slots:
	void R_timeout();
	void R_ShowAccountCheck(QWidget * wnd);
    void R_OnQuit();
    void R_QuitApplication();
    void R_ForceLogout();




private:
    WSLoginWnd  *m_loginwnd;
    BaseWnd *m_baseloginwnd;
    int          m_isShowCheck;

	QTimer   *  m_timeout;
};
