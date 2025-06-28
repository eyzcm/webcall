#ifndef SESSIONSERVICEIMPL_H
#define SESSIONSERVICEIMPL_H

#include "utils/Plat_EnumDefine.h"

#include "data/UILoginData.h"
#include "data/UIUserInfo.h"
#include "data/CommInfo.h"
#include <GlobalConfig.h>
#include <QObject>
#include <data/LicenceSetting.h>
#include <session/el_sessioninterface.h>

#include "Plat_view.h"


class LoginServiceImpl;


class SessionServiceImpl : public QObject
{
    Q_OBJECT

public:
    SessionServiceImpl();
    ~SessionServiceImpl();


	QWidget *   GetMainWnd();
	void SetMainWnd(QWidget * wnd);

    LoginServerSignal * LoginSignal();



    void DoLogoffProtocol();
    void DoLoginOk(int type );
    int  GetLoginErrorCount();
    void InitFirstLogin();

    ELOGINSTATUS Status();
    void         Status(ELOGINSTATUS status);
    QString      Token()
    {
        return m_token;
    };
	QString      LoginToken()
	{
		return m_logintoken;
	};
	

	QString      Account()
	{
		return m_account;
	};

    QString GetValueByKey(QString key);
    void    SetValueByKey(QString key, QString value);

    UIUserInfo* UserInfo();
    void        UserInfo(UIUserInfo* userinfo);

    void         LoginData(UILoginData* info);
    UILoginData* LoginData();

    QString Version(){ return m_version;}
    void Version( QString ver) {m_version = ver;};

    QString CurrentVersion()
    {
        return m_currentVersion;
    };
    int  Login(int flag);
    void SetUserPass(QString name, QString passwd, int flag);

    int           GetErrorCode();
    QString       GetErrorMsg();


    bool RetryLoginTag() const;
    void RetryLoginTag(bool val);
    bool OKLoginTag() const;
    void OKLoginTag(bool val);

    void SetCurrentStauts(ELOGINSTATUS status );

    CommInfo                *LocalConfig();
    QList<CommInfo *>        GetAccountList();

    void LoadLocalConfig();

    void SetLocalConfig(const CommInfo &info);

    void ModifyLocalSetting(UILoginData *data);

    void SaveLocalSetting();



    CommInfo           *GetAccountById(QString userId);
    void                DelectAccount(QString userId);

    void                SaveAccountLocConfig();



    LicenceSetting     *GetLicenceSetting() const;

    void CreateGlobalConfig(GlobalConfig config);

    GlobalConfig       *GetGlobalConfig() const;
    void SetLicenceSetting(LicenceSetting config);


    PermissionsSetting *GetPermissionsSetting() const;
    void SetPermissionsSetting(PermissionsSetting config);
    bool HasPermissionByKey(const QString strKey);

    QString GetSystemConfigByKey(const QString &key, const QString &defaultString);

    El_MenuNavInterface *  GetMenuNavWnd();
    ColleagueMgrInterface * GetColleagueService();

signals:
    void S_status(int newStatus, int oldStatus);

    void S_LoginOk(int type );
private slots:
    void R_SetCurrentStauts();

private:
    void LoadSystemConfig();
private:
    ELOGINSTATUS m_status;
    QString      m_token;

    QString m_currentVersion;

    QString m_account;

    QString m_password;
    QString m_logintoken;

    int               m_firstlogin;
    LoginServiceImpl* m_loginserviceImpl;

    QTimer* m_firstLoginTimer;

    UIUserInfo  m_nullUserInfo;
    UILoginData m_nullLoginData;

    QMap<QString, QString> m_extMap;

    bool m_isRetryLoginTag;
    bool m_isOKLoginTag;

    CommInfo         *m_otherConfig;
    CommInfo         *m_systemConfig;
    QList<CommInfo *> m_accountList;
    CommInfo         *m_currentAccount;

    GlobalConfig       *m_globalConfig;
    LicenceSetting     *m_licenceSetting;

    PermissionsSetting *m_permissionsSetting;

	QWidget *  m_mainWnd;


    Plat_view * m_menunav_view;

    ELOGINSTATUS m_newStauts;

    QString m_version;


};

#endif // SESSIONSERVICEIMPL_H
