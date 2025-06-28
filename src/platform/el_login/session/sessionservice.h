#ifndef SESSIONSERVICE_H
#define SESSIONSERVICE_H

#include "utils/Plat_EnumDefine.h"

#include "session/loginservice.h"
#include "data/UILoginData.h"
#include "data/UIUserInfo.h"
#include "data/GlobalConfig.h"
#include "data/LicenceSetting.h"
#include "session/el_sessionservice.h"
#include <QObject>

#include "tcp/ElConnectState.h"
#include <QMutex>
#include "CommInfo.h"
#include <data/UIVisitorUploadDataInfo.h>
#include "session/El_MenuNavInterface.h"
class El_login;

class SessionServiceImpl;
class  SessionService : public el_sessionInterface
{
    Q_OBJECT
private:
    SessionService();

public:
    ~SessionService();
    static SessionService* m_instance;
    static SessionService* GetInstance();



    virtual QString Version();
    virtual  void Version( QString ver) ;

    virtual void runLogin(QString param);
    virtual void ShowAccountCheck( QWidget * wnd  );
	virtual QWidget *   GetMainWnd() ;
	virtual void SetMainWnd(QWidget * wnd);

    virtual QString   GetSystemConfigByKey(const QString& key, const QString& defaultString);

    virtual El_MenuNavInterface *  GetMenuNavWnd();
    virtual ColleagueMgrInterface * GetColleagueService();

    void DoLogoffProtocol();
    ELOGINSTATUS Status();

    int         GetLoginErrorCount();
    QString     Token();
    UIUserInfo* UserInfo();
    void        UserInfo(UIUserInfo*);

    void         LoginData(UILoginData* info);
    UILoginData* LoginData();

    QString CurrentVersion();
    int     Login(int flag);
    void    SetUserPass(QString name, QString passwd, int flag);

    int           GetErrorCode();
    QString       GetErrorMsg();
    LoginServerSignal* LoginSignal();

    void InitFirstLogin();

    QString GetValueByKey(QString key);
    void    SetValueByKey(QString key, QString value);


    bool RetryLoginTag() const;
    void RetryLoginTag(bool val);
    bool OKLoginTag() const;
    void OKLoginTag(bool val);
    void SetCurrentStauts(ELOGINSTATUS status );


    void LoadLocalConfig();

    void SetLocalConfig(const CommInfo& info);

    void ModifyLocalSetting(UILoginData* data);

    void SaveLocalSetting();



    CommInfo* LocalConfig();

    QList<CommInfo*> GetAccountList();
    CommInfo*        GetAccountById(QString userId);
    void             DelectAccount(QString userId);


    void CreateGlobalConfig(GlobalConfig config);

    GlobalConfig       *GetGlobalConfig() ;
    void SetLicenceSetting(LicenceSetting config);
    LicenceSetting     *GetLicenceSetting();



    PermissionsSetting *GetPermissionsSetting() ;
    void SetPermissionsSetting(PermissionsSetting config);
    bool HasPermissionByKey(const QString strKey);

    UIVisitorUploadDataInfo * CreateUploadData();



signals:
    void S_MainTcpLoginStateChanged(ELTCPlLOGINSTATUS oldStatus, ELTCPlLOGINSTATUS newStatus);
    void S_ForceLogout();
    void S_ForceReLogin(int);



private:
    void                Status(ELOGINSTATUS status);
    SessionServiceImpl* m_pserveice;
    QMutex              m_mutex;

};

//#define gSessionService SessionService::GetInstance()

#endif // SESSIONSERVICE_H
