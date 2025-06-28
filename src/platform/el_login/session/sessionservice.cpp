#include "sessionservice.h"
#include "sessionserviceImpl.h"

#include <protocol/RequestLogoff.h>
#include "utils/utilshelper.h"
#include <data/UIVisitorUploadDataInfo.h>
#include "el_login.h"



SessionService*     SessionService::m_instance = nullptr;
SessionServiceImpl* gSessionServerImpl         = nullptr;

SessionService::SessionService()
{
    
    m_pserveice        = new SessionServiceImpl();
    gSessionServerImpl = m_pserveice;
    connect(m_pserveice,SIGNAL(S_LoginOk(int )), this,SIGNAL(S_LoginOk(int )) );

}

SessionService::~SessionService()
{
    if (m_pserveice)
        delete (m_pserveice);
}

SessionService* SessionService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SessionService;
    }
    return m_instance;
}

void SessionService::SetUserPass(QString name, QString passwd, int flag)
{
    QMutexLocker locker(&m_mutex);
    m_pserveice->SetUserPass(name, passwd, flag);
}


int SessionService::Login(int flag)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->Login(flag);
}

ELOGINSTATUS SessionService::Status()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->Status();
}

QString SessionService::Token()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->Token();
}

UIUserInfo* SessionService::UserInfo()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->UserInfo();
}

void SessionService::UserInfo(UIUserInfo* userinfo)
{
    QMutexLocker locker(&m_mutex);
    m_pserveice->UserInfo(userinfo);
}

QString SessionService::CurrentVersion()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->CurrentVersion();
}

void SessionService::Status(ELOGINSTATUS status)
{
    QMutexLocker locker(&m_mutex);

    m_pserveice->Status(status);
}

void SessionService::LoginData(UILoginData* info)
{
    QMutexLocker locker(&m_mutex);
    m_pserveice->LoginData(info);
}
UILoginData* SessionService::LoginData()
{
    QMutexLocker locker(&m_mutex);

    return m_pserveice->LoginData();
}

int SessionService::GetErrorCode()
{

    return m_pserveice->GetErrorCode();
}
QString SessionService::GetErrorMsg()
{

    return m_pserveice->GetErrorMsg();
}

LoginServerSignal* SessionService::LoginSignal()
{
    return m_pserveice->LoginSignal();
}

void SessionService::InitFirstLogin()
{
    return m_pserveice->InitFirstLogin();
}

int SessionService::GetLoginErrorCount()
{
    return m_pserveice->GetLoginErrorCount();
}

QString SessionService::GetValueByKey(QString key)
{
    return m_pserveice->GetValueByKey(key);
}

void SessionService::SetValueByKey(QString key, QString value)
{
    m_pserveice->SetValueByKey(key, value);
}

bool SessionService::RetryLoginTag() const
{
    return m_pserveice->RetryLoginTag();
}
void SessionService::RetryLoginTag(bool val)
{
    return m_pserveice->RetryLoginTag(val);
}
bool SessionService::OKLoginTag() const
{
    return m_pserveice->OKLoginTag();
}
void SessionService::OKLoginTag(bool val)
{
    return m_pserveice->OKLoginTag(val);
}


void SessionService::DelectAccount(QString userId)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->DelectAccount(userId);
}

CommInfo* SessionService::GetAccountById(QString userId)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->GetAccountById(userId);
}



void SessionService::LoadLocalConfig()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->LoadLocalConfig();
}

void SessionService::SetLocalConfig(const CommInfo& info)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->SetLocalConfig(info);
}

void SessionService::ModifyLocalSetting(UILoginData* data)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->ModifyLocalSetting(data);
}

void SessionService::SaveLocalSetting()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->SaveLocalSetting();
}

CommInfo* SessionService::LocalConfig()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->LocalConfig();
}

QList<CommInfo*> SessionService::GetAccountList()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->GetAccountList();
}



GlobalConfig *SessionService::GetGlobalConfig()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->GetGlobalConfig();
}
LicenceSetting *SessionService::GetLicenceSetting()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->GetLicenceSetting();
}

void SessionService::SetLicenceSetting(LicenceSetting config)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->SetLicenceSetting(config);
}

void SessionService::CreateGlobalConfig(GlobalConfig config)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->CreateGlobalConfig(config);
}

PermissionsSetting *SessionService::GetPermissionsSetting()
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->GetPermissionsSetting();
}

void SessionService::SetPermissionsSetting(PermissionsSetting config)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->SetPermissionsSetting(config);
}

bool SessionService::HasPermissionByKey(const QString strKey)
{
    QMutexLocker locker(&m_mutex);
    return m_pserveice->HasPermissionByKey(strKey);
}


void SessionService::DoLogoffProtocol()
{
    RequestLogoff req;
    QString       body = req.MakeXml();
    UtilsHelper::Execute(body);
}

QWidget *   SessionService::GetMainWnd()
{
	QMutexLocker locker(&m_mutex);
	return m_pserveice->GetMainWnd();
}

void SessionService::SetMainWnd(QWidget * wnd)
{
	QMutexLocker locker(&m_mutex);
	return m_pserveice->SetMainWnd(wnd);
}


UIVisitorUploadDataInfo *SessionService::CreateUploadData()
{
    UIVisitorUploadDataInfo *info = new UIVisitorUploadDataInfo();

    QDateTime dateTime = QDateTime::currentDateTime();
    // 转换成时间戳
    qint64 epochTime = dateTime.toMSecsSinceEpoch();
    info->CreateTime(QString("%1").arg(epochTime));

    info->CompanyId(UserInfo()->CompanyId());
    info->UserId(UserInfo()->UserId());
    info->UserName(UserInfo()->RealName());
    info->FirstType("2");
    if (UserInfo()->AiFlag())
    {
        info->Operator(4);
        info->UserType(2);
    }
    else
    {
        info->Operator(2);
        info->UserType(1);
    }
    return info;
}

QString   SessionService::GetSystemConfigByKey(const QString& key, const QString& defaultString)
{
    return m_pserveice->GetSystemConfigByKey(key,defaultString);
}

void SessionService::ShowAccountCheck( QWidget * wnd  )
{
    Q_EMIT S_ShowAccountCheck(wnd);
}


El_MenuNavInterface *  SessionService::GetMenuNavWnd(   )
{
    return m_pserveice->GetMenuNavWnd();
}

void SessionService::SetCurrentStauts(ELOGINSTATUS status )
{
    return m_pserveice->SetCurrentStauts(status );
}

void SessionService::runLogin(QString param)
{
    //todo 9999 是否有多个实例
    El_login * loginui = (El_login *)el_login_lib_GetDllInterface();

    if(loginui ) loginui->runLogin(param);

}

QString SessionService::Version()
{
    return m_pserveice->Version();
}

void SessionService::Version( QString ver)
{
    m_pserveice->Version(ver);
}

ColleagueMgrInterface * SessionService::GetColleagueService()
{
    return m_pserveice->GetColleagueService();
}
