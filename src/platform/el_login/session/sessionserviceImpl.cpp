#include "SessionServiceImpl.h"
#include "utils/ConstStringHelper.h"
#include "service/dbuiservice.h"


#include "protocol/platprotocolservice.h"

#include "session/loginserviceImpl.h"
#include "utils/UtilsHelper.h"
#include "utils/JsonHelper.h"

#include <protocol/RequestLogoff.h>
#include "service/util_service.h"
#include "service/permissionservice.h"
#include "service/ColleagueService.h"


SessionServiceImpl::SessionServiceImpl()
{
    m_menunav_view = nullptr;
	m_mainWnd = nullptr;
    m_permissionsSetting = nullptr;
    m_otherConfig = nullptr;
    m_systemConfig = nullptr;
    m_currentAccount = nullptr;
    m_globalConfig= nullptr;
    m_licenceSetting = nullptr;
    m_isRetryLoginTag =false;
    m_isOKLoginTag = false;

    m_firstlogin      = 0;
    m_firstLoginTimer = nullptr;

    m_status           = E_LOGIN_NONE;

    LoginServiceImpl * loginserviceImpl = new LoginServiceImpl(this);
    loginserviceImpl->Init();
    //增加loginservice服务
    m_loginserviceImpl   =  loginserviceImpl;
    LoadLocalConfig();
    LoadSystemConfig();
}

SessionServiceImpl::~SessionServiceImpl()
{
    if (m_systemConfig)
    {
        delete m_systemConfig;
        m_systemConfig = NULL;
    }
}

LoginServerSignal* SessionServiceImpl::LoginSignal()
{
    return m_loginserviceImpl->LoginSignal();
}



void SessionServiceImpl::SetUserPass(QString name, QString passwd, int flag)
{

    if (name.isEmpty() == false)
        m_account = name;
    if (flag == 0)
        m_password = passwd;
    else
    {
        m_logintoken = passwd;
        m_token = passwd;
    }
}

int SessionServiceImpl::Login(int flag)
{
    if ((m_account.length() > 0) && ((m_password.length() > 0) || (m_logintoken.length() > 0)))
    {
        if (m_loginserviceImpl->CurrentLoginState() == E_LOGIN_NONE)
        {
            m_firstlogin = true;
        }
        if ((m_loginserviceImpl->CurrentLoginState() == E_LOGIN_NONE) || (m_loginserviceImpl->CurrentLoginState() == E_LOGIN_ERROR) || (m_loginserviceImpl->CurrentLoginState() == E_LOGIN_PRE))
            m_loginserviceImpl->CurrentLoginState(E_LOGIN_DOING);

        if (flag == 1)
        {
            gIProtocolLoginAgent->DoExecLoginCmd(m_account, m_logintoken, flag, 0);
        }
        else if (flag == 2)
        {
            gIProtocolLoginAgent->DoExecLoginCmd(m_account, m_logintoken, 1, 1);
        }
        else
            m_loginserviceImpl->DoLogin();

        // gIProtocolAgent->DoExecLoginCmd(m_account, m_password,flag );
    }
    else
        return -1;

    return 0;
}

void SessionServiceImpl::Status(ELOGINSTATUS status)
{
    m_loginserviceImpl->CurrentLoginState(status);
}

ELOGINSTATUS SessionServiceImpl::Status()
{
    return m_loginserviceImpl->CurrentLoginState();
}

void SessionServiceImpl::LoginData(UILoginData* info)
{
    if (!m_password.isEmpty())
        info->Password(m_password);
    return gUIUserInfoManager->LoginData(info);
}
UILoginData* SessionServiceImpl::LoginData()
{
    UILoginData* loginData = gUIUserInfoManager->LoginData();
    if (loginData == NULL)
        return &m_nullLoginData;
    else
        return loginData;
}

UIUserInfo* SessionServiceImpl::UserInfo()
{
    UIUserInfo* info = gUIUserInfoManager->UserInfo();
    if (info == NULL)
        return &m_nullUserInfo;
    else
        return info;
}

void SessionServiceImpl::UserInfo(UIUserInfo* userinfo)
{

    UIUserInfo* tmpuserinfo = gUIUserInfoManager->CreateInfo(userinfo);
    m_token                 = tmpuserinfo->Token();
    gUIUserInfoManager->UserInfo(tmpuserinfo);
}

void SessionServiceImpl::InitFirstLogin()
{
    // return ;
    if (m_firstlogin)
    {
        m_firstlogin   = false;
        /*
        CommInfo* info = gUIPersonalConfigManager->GetUserOtherSetting();
        if (info)
        {
            QString aiuri;
            if (info->GetValueByKey("aiurl") == "1")
            {
                aiuri = gUISystemConfigManager->GetSystemConfigByKey("autoreplymessageurl_ai6", "https://ai6.eachbot.cn/api/robot/companyid");
            }
            else
            {
                aiuri = gUISystemConfigManager->GetSystemConfigByKey("autoreplymessageurl", "https://eachbot.easyliao.com/api/robot/companyid");
            }

            gIProtocolAgent->UpdateAIUrl(aiuri);


        }
        */
    }
}
int SessionServiceImpl::GetErrorCode()
{
    return gTcpManager->GetErrorCode();
}
QString SessionServiceImpl::GetErrorMsg()
{
    return gTcpManager->GetErrorMsg();
}


int SessionServiceImpl::GetLoginErrorCount()
{
    return m_loginserviceImpl->GetLoginErrorCount();
}

QString SessionServiceImpl::GetValueByKey(QString key)
{

    if (m_extMap.find(key) != m_extMap.end())
        return m_extMap[key];
    return "";
}

void SessionServiceImpl::SetValueByKey(QString key, QString value)
{
    if (m_extMap.find(key) != m_extMap.end())
    {
        m_extMap[key] = value;
    }
    else
        m_extMap.insert(key, value);
}

bool SessionServiceImpl::RetryLoginTag() const
{
    return m_isRetryLoginTag;
}
void SessionServiceImpl::RetryLoginTag(bool val)
{
    m_isRetryLoginTag = val;
}
bool SessionServiceImpl::OKLoginTag() const
{
    return m_isOKLoginTag;
}
void SessionServiceImpl::OKLoginTag(bool val)
{
    m_isOKLoginTag = val;
}


void SessionServiceImpl::LoadLocalConfig()
{
    QString     fileName = QString("%1/%2").arg(UtilsHelper::GetAppdataCacheDir()).arg("locSetting.conf");
    QString     val      = UtilsHelper::LoadFile(fileName);
    Json::Value value;
    bool        isOk = JsonHelper::GetJsonValueFromBody(val, value);
    if (isOk)
    {
        QMap<QString, QString> tempMap;
        bool                   isParse = JsonHelper::ParseValueToMap(value["currentaccount"], tempMap);
        if (isParse)
        {
            if (tempMap.find("password") != tempMap.end())
            {
                QString val = tempMap["password"];
                val         = QString::fromUtf8(QByteArray::fromBase64(val.toLocal8Bit()));
                tempMap.insert("password", val);
            }
            m_currentAccount = new CommInfo();
            m_currentAccount->AddCommInfo(tempMap);
        }

        Json::Value arrayJson = value["accountlist"];
        foreach (Json::Value valItem, arrayJson)
        {
            QMap<QString, QString> itemMap;
            bool                   isParse = JsonHelper::ParseValueToMap(valItem, itemMap);
            if (isParse)
            {
                if (itemMap.find("password") != itemMap.end())
                {
                    QString val = itemMap["password"];
                    val         = QString::fromUtf8(QByteArray::fromBase64(val.toLocal8Bit()));
                    itemMap.insert("password", val);
                }
                CommInfo *itemInfo = new CommInfo;
                itemInfo->AddCommInfo(itemMap);
                m_accountList.push_back(itemInfo);
            }
        }
    }
}

void SessionServiceImpl::SetLocalConfig(const CommInfo &info)
{
    if (m_currentAccount)
    {
        delete m_currentAccount;
        m_currentAccount = new CommInfo(info);
    }
    SaveAccountLocConfig();
}

void SessionServiceImpl::ModifyLocalSetting(UILoginData *data)
{
    if ((data == NULL) || (data->LoginType() != 0))
    {
        return;
    }

    m_currentAccount->SetValueByKey("username", data->UserName());
    m_currentAccount->SetValueByKey("state", QString::number(data->UserState()));
    if (data->IsAutoLogin())
    {
        m_currentAccount->SetValueByKey("autologin", "1");
        m_currentAccount->SetValueByKey("rememberpassword", "1");
        m_currentAccount->SetValueByKey("password", data->Password());
    }
    else
    {
        m_currentAccount->SetValueByKey("autologin", "0");
        if (data->IsRemember())
        {
            m_currentAccount->SetValueByKey("rememberpassword", "1");
            m_currentAccount->SetValueByKey("password", data->Password());
        }
        else
        {
            m_currentAccount->SetValueByKey("rememberpassword", "0");
        }
    }
}

void SessionServiceImpl::SaveLocalSetting()
{
    /*{
        "autostrat": "1",
        "saveusername": "1",
        "autologin": "1",
        "autoleavestate": "1",
        "username": "shizhenyu",
        "password": "123456"
    }*/
    UILoginData *data = gUIUserInfoManager->LoginData();
    if (data)
    {
        if (data->LoginType() == 0)
        {
            if (m_currentAccount == nullptr)
            {
                m_currentAccount = new CommInfo();
                m_currentAccount->SetValueByKey("autoleavestate", "");
            }

            ModifyLocalSetting(data);
            m_currentAccount->SetValueByKey("userpicture", "");
            m_currentAccount->SetValueByKey("nickname", gUIUserInfoManager->UserInfo()->GetDisplayName());
        }
    }
    SaveAccountLocConfig();
}
void SessionServiceImpl::SaveAccountLocConfig()
{
    if (m_currentAccount == nullptr)
    {
        return;
    }
    Json::Value            val;
    Json::Value            valitem0;
    Json::Value            arrayVal;
    QMap<QString, QString> tempMap = m_currentAccount->InfoMap();
    for (auto itor = tempMap.begin(); itor != tempMap.end(); itor++)
    {
        QString key         = itor.key();
        QString valPassword = itor.value();
        if (key == "password")
        {
            valPassword = QString::fromLocal8Bit(valPassword.toUtf8().toBase64());
        }

        QString keyTemp                               = "currentaccount";
        val[keyTemp.toStdString()][key.toStdString()] = valPassword.toStdString();
        valitem0[key.toStdString()]                   = valPassword.toStdString();
    }
    int i       = 0;
    arrayVal[i] = valitem0;
    i++;
    Q_FOREACH (CommInfo *itemInfo, m_accountList)
    {
        if (itemInfo)
        {
            if (itemInfo->GetValueByKey("username") != m_currentAccount->GetValueByKey("username"))
            {
                /*if (i>8)
                                {
                                        break;
                }*/
                Json::Value            valitem;
                QMap<QString, QString> itemMap = itemInfo->InfoMap();
                for (auto itor = itemMap.begin(); itor != itemMap.end(); itor++)
                {
                    QString key = itor.key();
                    QString val = itor.value();
                    if (key == "password")
                    {
                        val = QString::fromLocal8Bit(val.toUtf8().toBase64());
                    }
                    valitem[key.toStdString()] = val.toStdString();
                }
                arrayVal[i] = valitem;
                i++;
            }
        }
    }

    val["accountlist"] = arrayVal;

    std::string out      = val.toStyledString();
    QString     fileName = QString("%1/%2").arg(UtilsHelper::GetAppdataCacheDir()).arg("locSetting.conf");
    UtilsHelper::SaveFile(fileName, QString::fromStdString(out));
}

CommInfo *SessionServiceImpl::LocalConfig()
{
    return m_currentAccount;
}

QList<CommInfo *> SessionServiceImpl::GetAccountList()
{
    return m_accountList;
}


void SessionServiceImpl::DelectAccount(QString userId)
{
    Q_FOREACH (CommInfo *info, m_accountList)
    {
        if (info)
        {
            if (info->GetValueByKey("username") == userId)
            {
                m_accountList.removeOne(info);
                delete info;
                break;
            }
        }
    }
}
CommInfo *SessionServiceImpl::GetAccountById(QString userId)
{
    CommInfo *ret = nullptr;
    Q_FOREACH (CommInfo *info, m_accountList)
    {
        if (info)
        {
            if (info->GetValueByKey("username") == userId)
            {
                ret = info;
                break;
            }
        }
    }
    return ret;
}


GlobalConfig *SessionServiceImpl::GetGlobalConfig() const
{
    return m_globalConfig;
}

LicenceSetting *SessionServiceImpl::GetLicenceSetting() const
{
    return m_licenceSetting;
}


void SessionServiceImpl::CreateGlobalConfig(GlobalConfig config)
{
    if (m_globalConfig == NULL)
    {
        m_globalConfig = new GlobalConfig(config);
    }
    else
    {
        delete m_globalConfig;
        m_globalConfig = NULL;
        m_globalConfig = new GlobalConfig(config);
    }
}


void SessionServiceImpl::SetLicenceSetting(LicenceSetting config)
{
    if (m_licenceSetting == NULL)
    {
        m_licenceSetting = new LicenceSetting(config);
    }
    else
    {
        delete m_licenceSetting;
        m_licenceSetting = NULL;
        m_licenceSetting = new LicenceSetting(config);
    }
}

void SessionServiceImpl::DoLoginOk(int type)
{
    Q_EMIT S_LoginOk( type );

}


PermissionsSetting *SessionServiceImpl::GetPermissionsSetting() const
{
    return m_permissionsSetting;
}



void SessionServiceImpl::SetPermissionsSetting(PermissionsSetting config)
{
    if (m_permissionsSetting == NULL)
    {
        m_permissionsSetting = new PermissionsSetting(config);
    }
    else
    {
        delete m_permissionsSetting;
        m_permissionsSetting = NULL;
        m_permissionsSetting = new PermissionsSetting(config);
    }

}


bool SessionServiceImpl::HasPermissionByKey(const QString strKey)
{
    if (m_permissionsSetting)
    {
        return m_permissionsSetting->HasPermissionByKey(strKey);
    }
    return false;
}


void SessionServiceImpl::DoLogoffProtocol()
{
    RequestLogoff req;
    QString       body = req.MakeXml();
    UtilsHelper::Execute(body);
}


QWidget *   SessionServiceImpl::GetMainWnd()
{
	
	return m_mainWnd;
}

void SessionServiceImpl::SetMainWnd(QWidget * wnd)
{
	m_mainWnd = wnd;
	
}


void SessionServiceImpl::LoadSystemConfig()
{
    QString     fileName = QString("%1/%2").arg(UtilsHelper::GetRunAppDir()).arg("system.conf");
    QString     val      = UtilsHelper::LoadFile(fileName);
    Json::Value value;
    bool        isOk = JsonHelper::GetJsonValueFromBody(val, value);
    if (isOk)
    {
        QMap<QString, QString> tempMap;
        bool                   isParse = JsonHelper::ParseValueToMap(value, tempMap);
        if (isParse)
        {
            CommInfo info;
            info.AddCommInfo(tempMap);
            m_systemConfig = new CommInfo(info);
			
			QString islogType = info.GetValueByKey("islogType");
            QString islog  = info.GetValueByKey("islog");
			if (!islog.isEmpty())
			{
				if(islogType == "1")
					gLogServer->SetIsLog(2);
				else
					gLogServer->SetIsLog(islog.toUInt());
			}

        }
    }
}

QString SessionServiceImpl::GetSystemConfigByKey(const QString &key, const QString &defaultString)
{
    QString ret = defaultString;
    do
    {
        if (m_systemConfig)
        {
            QString val = m_systemConfig->GetValueByKey(key);
            if (!val.isEmpty())
            {
                ret = val;
                break;
            }
        }
    } while (false);

    return ret;
}

El_MenuNavInterface *  SessionServiceImpl::GetMenuNavWnd()
{
    if( m_menunav_view == nullptr)
    {
        m_menunav_view = new Plat_view();

		
		//m_menunav_view->MenuNav()->SetMenuData("addd", );
        QList<UIMenuDataInfo *> menuinfolist = gPermissionSerice->GetFunMenuList();

        foreach( UIMenuDataInfo * menuinfo,menuinfolist)
        {
            m_menunav_view->MenuNav()->SetMenuData(menuinfo->Code(),menuinfo );
        }



    }
    return m_menunav_view->MenuNav();
}

void SessionServiceImpl::SetCurrentStauts(ELOGINSTATUS status )
{
    m_newStauts = status;
    QTimer::singleShot(300, this, SLOT(R_SetCurrentStauts()));
}

void SessionServiceImpl::R_SetCurrentStauts()
{
    m_loginserviceImpl->CurrentLoginState( m_newStauts );
}

ColleagueMgrInterface * SessionServiceImpl::GetColleagueService()
{
    return gcolleagueSerice;
}

