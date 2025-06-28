#ifndef EL_SESSIONINTERFACE_H
#define EL_SESSIONINTERFACE_H

#include <QObject>
#include <data/PermissionsSetting.h>
#include "data/UILoginData.h"
#include "data/UIUserInfo.h"
#include "data/GlobalConfig.h"
#include "data/LicenceSetting.h"
#include "el_session_global.h"
#include "El_MenuNavInterface.h"
#include "ColleagueMgrInterface.h"

#include "data/UIVisitorUploadDataInfo.h"



class EL_SESSION_EXPORT  LoginServerSignal : public QObject
{
    Q_OBJECT

public:
    LoginServerSignal(){};
    ~LoginServerSignal(){};
signals:
    void S_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void S_MainTcpNetWorkError(ELOGINSTATUS loginStatus, int errorCode, QString msg);
    void S_Relogin();
    void S_ForceLogout();
    void S_ForceReLogin(int);
	void S_ShowAccountCheck(QWidget * wnd);
};


class  EL_SESSION_EXPORT el_sessionInterface :public QObject
{
    Q_OBJECT
public:
    el_sessionInterface();
    ~el_sessionInterface();



	virtual QString Version() = 0;
    virtual  void Version( QString ver) = 0;

    virtual void runLogin(QString param) = 0;
	virtual QWidget *   GetMainWnd() = 0;
    virtual void SetMainWnd(QWidget * wnd) = 0;
    virtual void ShowAccountCheck( QWidget * wnd  ) = 0;

    virtual void  DoLoginReturnData(QString body){ Q_EMIT S_LoginReturnData(body); };

    virtual void DoLogoffProtocol() = 0;
    virtual ELOGINSTATUS Status(){return E_LOGIN_NONE;};

    virtual  int         GetLoginErrorCount() = 0;
    virtual QString     Token() = 0;
    virtual UIUserInfo* UserInfo() = 0;
    virtual void        UserInfo(UIUserInfo*) = 0;

    virtual void         LoginData(UILoginData* info) = 0;
    virtual UILoginData* LoginData() = 0;

    virtual QString CurrentVersion() = 0;
    virtual int     Login(int flag) = 0;
    virtual void    SetUserPass(QString name, QString passwd, int flag) = 0;

    virtual int           GetErrorCode()= 0;
    virtual QString       GetErrorMsg() = 0;
    virtual LoginServerSignal* LoginSignal() = 0;

    virtual void InitFirstLogin() = 0;

    virtual QString GetValueByKey(QString key) = 0;
    virtual void    SetValueByKey(QString key, QString value) = 0;


    virtual bool RetryLoginTag() const = 0;
    virtual void RetryLoginTag(bool val) = 0;;
    virtual bool OKLoginTag() const = 0;
    virtual void OKLoginTag(bool val) = 0;

    virtual void SetCurrentStauts(ELOGINSTATUS status ) = 0;


    virtual void LoadLocalConfig() = 0;

    virtual void SetLocalConfig(const CommInfo& info) = 0;

    virtual void ModifyLocalSetting(UILoginData* data) = 0;

    virtual void SaveLocalSetting() = 0;
    virtual LicenceSetting *GetLicenceSetting() = 0;



    virtual CommInfo* LocalConfig() = 0;

    virtual  QList<CommInfo*> GetAccountList() = 0;;
    virtual CommInfo*        GetAccountById(QString userId) = 0;
    virtual void             DelectAccount(QString userId) = 0;


    virtual void CreateGlobalConfig(GlobalConfig config) = 0;

    virtual GlobalConfig       *GetGlobalConfig() = 0;
    virtual void SetLicenceSetting(LicenceSetting config) = 0;

    virtual PermissionsSetting *GetPermissionsSetting() = 0;
     virtual void SetPermissionsSetting(PermissionsSetting  config) = 0;
    virtual bool HasPermissionByKey(const QString strKey) = 0;
    virtual UIVisitorUploadDataInfo * CreateUploadData() = 0;


    virtual void DoQuitApplication(){ Q_EMIT S_QuitApplication(); };
    virtual QString   GetSystemConfigByKey(const QString& key, const QString& defaultString) = 0;

    virtual El_MenuNavInterface *  GetMenuNavWnd() = 0;
    virtual ColleagueMgrInterface * GetColleagueService() = 0;

signals:
    void S_LoginOk(int type );
    void S_LoginReturnData(QString body);
    void S_QuitApplication();
    void S_ShowAccountCheck( QWidget * wnd  );

};

#endif // EL_SESSIONINTERFACE_H
