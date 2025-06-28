#include "loginserviceImpl.h"
#include "utils/DefineHelper.h"
#include "QObject"
#include "SessionServiceImpl.h"

#include "service/dbuiservice.h"
#include "include/json/reader.h"
#include "utils/jsonhelper.h"
//#include "model/UICommonDataModel.h"
/*
#include "protocol/visitormonitortcpmanager.h"
#include "protocol_biz/Protocol_Login_AgentImp.h"
#include "protocol_biz/protocolloginwork.h"
#include "protocol_biz/tcploginagent.h"
*/
#include "protocol/platprotocolservice.h"

#include "session/el_sessionservice.h"
#include "utils/EL_Util_Time.h"
#include "utils/utilshelper.h"
#include "data/GlobalConfig.h"
#include "data/LicenceSetting.h"
#include "session/el_sessionservice.h"
#include "service/util_service.h"
#include "db/platdbservice.h"




LoginServiceImpl::LoginServiceImpl(SessionServiceImpl *main)
{
    m_currentLoginState   = E_LOGIN_NONE;
    m_main                = main;
    m_protocol            = gIProtocolLoginAgent;
    m_clientType          = E_CLIENT_NEWMEDICAL;

    m_visitorMonitorTimer = NULL;
    m_vmLogined           = 0;
    m_reLoginTimer        = nullptr;
    m_loginErrorCount     = 0;
    m_logingServerSignal = new LoginServerSignal();

    connect(this, SIGNAL(S_LoginStateChanged(ELOGINSTATUS , ELOGINSTATUS )), m_logingServerSignal, SIGNAL(S_LoginStateChanged(ELOGINSTATUS , ELOGINSTATUS )));
    connect(this, SIGNAL(S_MainTcpNetWorkError(ELOGINSTATUS , int , QString )), m_logingServerSignal, SIGNAL(S_MainTcpNetWorkError(ELOGINSTATUS , int , QString )));
    connect(this, SIGNAL(S_Relogin()), m_logingServerSignal, SIGNAL(S_Relogin()));
    connect(this, SIGNAL(S_ForceLogout()), m_logingServerSignal, SIGNAL(S_ForceLogout()));

}

LoginServiceImpl::~LoginServiceImpl()
{
}

void LoginServiceImpl::LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus)
{
    Q_EMIT S_LoginStateChanged(oldStatus, newStatus);
}
void LoginServiceImpl::MainTcpNetWorkError(ELOGINSTATUS loginStatus, int errorCode, QString msg)
{
    Q_EMIT S_MainTcpNetWorkError(loginStatus, errorCode, msg);
}
void LoginServiceImpl::MainTcpLoginFinished(QString authType, QString errorCode, QString body)
{
    Q_EMIT S_MainTcpLoginFinished(authType, errorCode, body);
}
void LoginServiceImpl::VM_LoginEnd()
{
    Q_EMIT S_VM_LoginEnd();
}

void LoginServiceImpl::MainTcpConnected()
{
    Q_EMIT S_MainTcpConnected();
}

void LoginServiceImpl::Init()
{
    connect(this, SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
    connect(this, SIGNAL(S_MainTcpConnected()), this, SLOT(R_MainTcpConnected()));
    connect(m_protocol, SIGNAL(S_MainTcpLoginFinished(QString, QString, QString)), this, SLOT(R_MainTcpLoginFinished(QString, QString, QString)));
    connect(m_protocol, SIGNAL(S_ForceLogout()), this, SLOT(R_ForceLogout()));
    connect(m_protocol, SIGNAL(S_ForceReLogin(int)), this, SIGNAL(S_ForceReLogin(int)));

    connect(m_protocol, SIGNAL(S_MainTcpConnected()), this, SLOT(R_MainTcpConnected()));
    connect(m_protocol, SIGNAL(S_MainTcpNetWorkError(int, QString)), this, SLOT(R_MainTcpNetWorkError(int, QString)));

    connect(m_protocol, SIGNAL(S_VMConnected()), this, SLOT(R_VMConnected()));
    connect(m_protocol, SIGNAL(S_VM_AuthFinished(bool, int)), this, SLOT(R_VM_AuthFinished(bool, int)));
    connect(m_protocol, SIGNAL(S_VMMessageParsingFailed()), this, SLOT(R_VMMessageParsingFailed()));
    connect(m_protocol, SIGNAL(S_VM_LoginFinished(QString)), this, SLOT(R_VM_LoginFinished(QString)));

    connect(this, SIGNAL(S_AdminAuthLogin()), gIProtocolLoginAgent, SIGNAL(S_AdminAuthLogin()));

}

void LoginServiceImpl::R_LoginStateChanged(ELOGINSTATUS oldState, ELOGINSTATUS newState)
{

    m_protocol->DoLoginStateChanged(oldState, newState);
}

void LoginServiceImpl::R_MainTcpConnected()
{
    GLOG("R_MainTcpConnected is success!");
}

void LoginServiceImpl::R_MainTcpNetWorkError(int errorCode, QString msg)
{
    QString info = QString(tr("网络错误，错误码：%1  错误描述：%2")).arg(errorCode).arg(msg);
    GERRORLOG(info);
    ELOGINSTATUS loginStatus = m_main->Status();
    if (m_main->Status() == E_LOGIN_OK_MAINTCP || m_main->Status() == E_LOGIN_ALLOK)
    {
        m_main->Status(E_LOGIN_RETRY_MAINTCP);
    }
    if (m_main->Status() == E_LOGIN_RETRY_MAINTCP || m_main->Status() == E_LOGIN_ERROR_MONITORTCP || m_main->Status() == E_LOGIN_PRE_MONITORTCP || m_main->Status() == E_LOGIN_RETRY_MONITORTCP)
    {
        m_main->Status(E_LOGIN_RETRY_MAINTCP);
    }
    else if (m_main->Status() == E_LOGIN_LOGOUT)
    {
        // do noting.
    }
    else
    {
        if (m_main->Status() == E_LOGIN_DOING)
        {
            m_main->Status(E_LOGIN_NONE);
        }
        else
        {
            QString isAutoReply = GetSystemConfigByKey("customeraitype", "0");
            if (isAutoReply == "1")
            {
                m_main->Status(E_LOGIN_RETRY_MAINTCP);
            }
            else
            {
                if (gSessionService->RetryLoginTag())
                {
                    m_main->Status(E_LOGIN_RETRY_MAINTCP);
                }
                else
                {
                    if (gSessionService->OKLoginTag())
                    {
                        m_main->Status(E_LOGIN_RETRY_MAINTCP);
                    }
                    else
                    {
                    }
                }
            }
        }
    }
    Q_EMIT S_MainTcpNetWorkError(loginStatus, errorCode, msg);
}
#if 0

void LoginServiceImpl::R_MainTcpLoginFinished(QString authType, QString errorCode, QString body)
{

	GTESTLOG(QString("R_MainTcpLoginFinished,%1").arg(authType));
	if (authType == "error")
	{
		
		QString tbody = "{\"userInfo\":{\"realName\":\" 18910689141\",\"companyId\":11211,\"departmentId\":40561,\"nikeName\":\"18910689141\",\"userName\":\"18910689141\",\"type\":2,\"noticeTip\":\"\",\"token\":\"${token}\",\"status\":1},\"licence\":{\"expired\":1701333076000,\"limitOnlineCount\":500},\"mobileHiddenTime\":0,\"permissions\":[\"CARD_ALLOCATION\",\"chatMonitor\",\"summaryLabel\",\"Monitoring statistics\",\"chatRecord\",\"csMonitor\",\"commonWord\",\"visitorManager\",\"leaveMessage\",\"visitorCard\",\"seizeChat\",\"AIRobot\",\"msgWithdraw\",\"registryForm\",\"console\",\"visitorAnalyse\",\"AI\",\"a16abc4e-4b5b-4570-8980-7df34294f5e5\",\"aiSetting\",\"10000:AuthOpen\",\"colleague\",\"miniHTML\",\"excludeVisitor\",\"freePhone\",\"report\",\"visitorMonitor\",\"transchat\",\"CRM\",\"clues\"],\"timeoutReplay\":{\"replyTimerOfCustomer\":\"10\",\"replyMsgAtConnected\":\"\",\"replyMsgOfVisitor\":\"已经很久没有收到您的消息了，请问您还在电脑前吗？如果没有其他的问题我将主动关闭该对话。随时欢迎您再次向我咨询，祝您今天好心情。\",\"replyTimerOfVisitor\":\"10\",\"replyMsgOfCustomer\":\"我现在临时有事需要离开电脑前，您可以先浏览一下网站看看产品的详细介绍，或者留下您的联系方式等我回复，给您带来的不便请多谅解。\",\"autoReplyAtConnected\":\"0\"},\"tabs\":[{\"name\":\"在线客服\",\"type\":\"webcall\",\"url\":\"\"},{\"name\":\"管理中心\",\"type\":\"webview\",\"url\":\"http://group-prd3-mgr.easyliao.com/console/Service/LoginConsole\"},{\"name\":\"统计分析\",\"type\":\"webview\",\"url\":\"http://group-prd3-mgr.easyliao.com/report/Service/LoginReport\"},{\"name\":\"过程诊断\",\"type\":\"webview\",\"url\":\"http://group-prd3-mgr.easyliao.com/report/Service/LoginReport\"},{\"name\":\"对话记录\",\"type\":\"webview\",\"url\":\"http://group-prd3-mgr.easyliao.com/record/Service/LoginRecord\"},{\"name\":\"CRM\",\"type\":\"webview\",\"url\":\"http://group-prd3-mgr.easyliao.com/crm/Service/LoginReportCrm\"}],\"exts\":\"杨荣测试,yangrong_key,康熙测试,summary,百度推广账号id,baidu_aduserid,流量渠道,baidu_channel\",\"summarizeOnClose\":\"0\",\"globalConf\":{\"fileUploadURL\":\"http://group-prd3-live.easyliao.com/live/fileupload/fileupload\",\"reportLoginURL\":\"http://group-prd3-mgr.easyliao.com/report/Service/LoginReport\",\"consoleLoginURL\":\"http://group-prd3-mgr.easyliao.com/console/Service/LoginConsole\",\"visitorLoginURL\":\"http://group-prd3-mgr.easyliao.com/report/Service/LoginReport\",\"clientApiURL\":\"http://group-prd3-mgr.easyliao.com/client/api\",\"crmLoginURL\":\"http://group-prd3-mgr.easyliao.com/crm/Service/LoginReportCrm\",\"dumpUploadURL\":\"http://live.easyliao.com/live/dumpupload.jsp\",\"recordLoginURL\":\"http://group-prd3-mgr.easyliao.com/record/Service/LoginRecord\",\"flashUrl\":\"http://group-prd3-live.easyliao.com/live/\",\"wxFileDownURL\":\"http://group-prd3-live.easyliao.com/live/down.jsp\"},\"mobileHidden\":0}";
		tbody = tbody.replace("18910689141", m_main->Account());
		tbody = tbody.replace("${token}", m_main->LoginToken());
		m_loginErrorCount = 0;
		if (m_main->Status() == E_LOGIN_DOING)
		{



			ParseLoginResult(tbody);


			UIUserInfo *info = m_main->UserInfo();
			m_main->DoLoginOk(1);
			m_main->Status(E_LOGIN_OK_MAINTCP);
			QString moniserver = GetSystemConfigByKey("visitormonitorurl", "im.easyliao.com");
			QString moniport = GetSystemConfigByKey("visitormonitorport", "1891");
			QString monitorvisitornumber = GetSystemConfigByKey("monitorvisitornumber", "50");

			if (gSessionService->HasPermissionByKey("visitorMonitor") == false)
			{
				this->CurrentLoginState(E_LOGIN_ALLOK);
			}
			else
			{
				m_protocol->InitVisitorMonitorTcpAgent(moniserver, moniport, info->UserId(), info->CompanyId(), monitorvisitornumber);
			}



		}
		else if (m_main->Status() == E_LOGIN_RETRY_MAINTCP)
		{

			ParseLoginResult(tbody);


			UIUserInfo *info = m_main->UserInfo();
			m_main->DoLoginOk(2);
			m_main->Status(E_LOGIN_ALLOK);
			QString moniserver = GetSystemConfigByKey("visitormonitorurl", "im.easyliao.com");
			QString moniport = GetSystemConfigByKey("visitormonitorport", "1891");

			QString monitorvisitornumber = GetSystemConfigByKey("monitorvisitornumber", "50");

			if (gSessionService->HasPermissionByKey("visitorMonitor") == false)
			{
				this->CurrentLoginState(E_LOGIN_ALLOK);
			}
			else
			{
				m_protocol->InitVisitorMonitorTcpAgent(moniserver, moniport, info->UserId(), info->CompanyId(), monitorvisitornumber);
			}

		}
		else
		{
			ParseLoginResult(body);
			m_main->DoLoginOk(3);

		}

		gTcpManager->LoginOk();
	}
	else if (authType == "error")
	{
		GTESTLOG(QString("R_MainTcpLoginFinished,errorbody=%1").arg(body));

		m_loginErrorCount++;
		CurrentLoginState(E_LOGIN_ERROR);

	}
	else
	{
		GTESTLOG(QString("R_MainTcpLoginFinished,error=%1,error=%1,body=%2").arg(errorCode).arg(body));
		if (errorCode == "301")
		{
			m_loginErrorCount++;
			m_main->Status(E_LOGIN_NONE);

			CreateReloginTimer();
			//	m_main->Login();//重新登录
			return;
		}
		else if (errorCode == "302")
		{
			m_main->Status(E_LOGIN_NONE);

			Q_EMIT S_AdminAuthLogin();
			return;
		}
		else
		{
			m_loginErrorCount++;
			Q_ASSERT(false);
		}
	}



	Q_EMIT S_MainTcpLoginFinished(authType, errorCode, body);
}


#else
void LoginServiceImpl::R_MainTcpLoginFinished(QString authType, QString errorCode, QString body)
{

    GTESTLOG( QString("R_MainTcpLoginFinished,%1").arg(authType)  );
    if (authType == "ok")
    {


		


        m_loginErrorCount = 0;
        if (m_main->Status() == E_LOGIN_DOING)
        {
            ParseLoginResult(body);


            UIUserInfo *info = m_main->UserInfo();
            QString moniserver =  GetSystemConfigByKey("visitormonitorurl","im.easyliao.com");
            QString moniport = GetSystemConfigByKey("visitormonitorport","1891");
            QString monitorvisitornumber = GetSystemConfigByKey("monitorvisitornumber", "50");

           if (gSessionService->HasPermissionByKey("visitorMonitor") == false)
            {
                ;//this->CurrentLoginState(E_LOGIN_ALLOK);
            }
            else
            {
                m_protocol->InitVisitorMonitorTcpAgent(moniserver, moniport, info->UserId(), info->CompanyId(), monitorvisitornumber);
            }
           GCACHEDBSERVICE->Init(info->UserId());
		   GWEBCACHEDBSERVICE->Init(info->UserId());
		   m_main->DoLoginOk(1);
		   m_main->Status(E_LOGIN_OK_MAINTCP);




        }
        else if (m_main->Status() == E_LOGIN_RETRY_MAINTCP)
        {

            ParseLoginResult(body);


            UIUserInfo *info = m_main->UserInfo();
            m_main->DoLoginOk(2  );
            m_main->Status(E_LOGIN_ALLOK);
            QString moniserver =  GetSystemConfigByKey("visitormonitorurl","im.easyliao.com");
            QString moniport = GetSystemConfigByKey("visitormonitorport","1891");

            QString monitorvisitornumber = GetSystemConfigByKey("monitorvisitornumber", "50");

           if (gSessionService->HasPermissionByKey("visitorMonitor") == false)
            {
                this->CurrentLoginState(E_LOGIN_ALLOK);
            }
            else
            {
                m_protocol->InitVisitorMonitorTcpAgent(moniserver, moniport, info->UserId(), info->CompanyId(), monitorvisitornumber);
            }

        }
        else
        {
            ParseLoginResult(body);
            m_main->DoLoginOk(3  );

        }

        gTcpManager->LoginOk();
    }
    else if (authType == "error")
    {
        GTESTLOG( QString("R_MainTcpLoginFinished,errorbody=%1").arg(body)  );

        m_loginErrorCount++;
        CurrentLoginState(E_LOGIN_ERROR);

    }
    else
    {
        GTESTLOG( QString("R_MainTcpLoginFinished,error=%1,error=%1,body=%2").arg(errorCode).arg(body)  );
        if (errorCode == "301")
        {
            m_loginErrorCount++;
            m_main->Status(E_LOGIN_NONE);

            CreateReloginTimer();
            //	m_main->Login();//重新登录
            return;
        }
        else if (errorCode == "302")
        {
            m_main->Status(E_LOGIN_NONE);

            Q_EMIT S_AdminAuthLogin();
            return;
        }
        else
        {
            m_loginErrorCount++;
            Q_ASSERT(false);
        }
    }



    Q_EMIT S_MainTcpLoginFinished(authType, errorCode, body);
}
#endif

void LoginServiceImpl::R_VMConnected()
{
    GLOG("R_VMConnected is success!");

    if (m_vmLogined == 0)
        m_main->Status(E_LOGIN_PRE_MONITORTCP);
    else
        m_main->Status(E_LOGIN_RETRY_MONITORTCP);
    m_vmLogined = 1;

    {
        m_protocol->DoExecVMAuthLogin();
    }

}

QString LoginServiceImpl::GetSystemConfigByKey(const QString &key, const QString &defaultString)
{


    QString val ;

    val = gSessionService->GetSystemConfigByKey(key, defaultString);

    return val;


}

void LoginServiceImpl::ParseLoginResult(QString body)
{
    Json::Reader reader;
    Json::Value  val;

    std::string bodyStr = body.toUtf8().data();

    if (reader.parse(bodyStr, val))
    {
        Json::Value TemplicenceData = val["licence"];
        if (TemplicenceData != Json::nullValue)
        {
            ParseLicenceData(TemplicenceData);
        }

        Json::Value timeoutReplay = val["timeoutReplay"];
        if (timeoutReplay != Json::nullValue)
        {
            ParseTimeoutReplayData(timeoutReplay);
        }

        Json::Value globalConf = val["globalConf"];
        if (globalConf != Json::nullValue)
        {
            ParseGlobalConfigData(globalConf);
        }


        Json::Value permission = val["permissions"];
        if (permission != Json::nullValue && permission.isArray())
        {
            PermissionsSetting permissionSetting;
            foreach (Json::Value permissionItem, permission)
            {
                if (permissionItem != Json::nullValue)
                {
                    permissionSetting.AddPermission(QString::fromStdString(permissionItem.asString()));
                }
            }
            gSessionService->SetPermissionsSetting(permissionSetting);
        }

		gSessionService->DoLoginReturnData(body);

        Json::Value userInfo = val["userInfo"];
        if (userInfo != Json::nullValue)
        {
            ParseUserInfoData(userInfo);
        }

    }
}

void LoginServiceImpl::ParseLicenceData(Json::Value val)
{
    LicenceSetting setting;
    if (val["expired"] != Json::nullValue)
    {
        double    expired  = val["expired"].asDouble();
        long long lexpired = expired;
        QString   strVal   = QString("%1").arg(lexpired);
        setting.Expired(strVal);
    }

    if (val["limitOnlineCount"] != Json::nullValue)
    {
        int limitOnlineCount = val["limitOnlineCount"].asInt();
        setting.LimitOnlineCount(QString::number(limitOnlineCount));
    }
    gSessionService->SetLicenceSetting(setting);
}

void LoginServiceImpl::ParseTimeoutReplayData(Json::Value val)
{
    /*
    QMap<QString, QString> tempMap;
    bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
    if (isParse)
    {
        AutoReplaySetting setting;

        setting.ReplyTimerOfCustomer(tempMap["replyTimerOfCustomer"]);
        setting.AutoReplyAtConnected(tempMap["autoReplyAtConnected"]);
        setting.ReplyMsgAtConnected(tempMap["replyMsgAtConnected"]);
        setting.ReplyTimerOfVisitor(tempMap["replyTimerOfVisitor"]);
        setting.ReplyMsgOfVisitor(tempMap["replyMsgOfVisitor"]);
        setting.ReplyMsgOfCustomer(tempMap["replyMsgOfCustomer"]);
        QString onOffFlag = tempMap["onOffFlag"];
        if (onOffFlag.isEmpty())
            onOffFlag = "7";
        setting.OnOffFlag(onOffFlag.toInt());

        gUIPersonalConfigManager->SetAutoReplaySetting(setting);
    }
    else
    {
        Q_ASSERT(false);
        GERRORLOG("TimeoutReplay配置解析错误。");
    }
    */
}

void LoginServiceImpl::ParseGlobalConfigData(Json::Value val)
{
    QMap<QString, QString> tempMap;
    bool                   isParse = JsonHelper::ParseValueToMap(val, tempMap);
    if (isParse)
    {
        GlobalConfig setting;
        setting.AddCommInfo(tempMap);
        gSessionService->CreateGlobalConfig(setting);
    }
    else
    {
        Q_ASSERT(false);
        GERRORLOG("全局配置解析错误。");
    }
}


void LoginServiceImpl::ParseUserInfoData(Json::Value val)
{
    UIUserInfo            *pinfo = new UIUserInfo();
    QMap<QString, QString> tempMap;
    bool                   isOk = JsonHelper::ParseValueToMap(val, tempMap);
    if (isOk)
    {
        pinfo->NickName(tempMap["nikeName"]);
        pinfo->Token(tempMap["token"]);
        pinfo->UserId(tempMap["userName"]);
        pinfo->RealName(tempMap["realName"]);
        pinfo->CompanyId(tempMap["companyId"]);
        pinfo->DepartmentId(tempMap["departmentId"]);

        // type为1表示人工 ，2表示机器人
        if (tempMap["type"] == "2")
        {
            pinfo->AiFlag(1);
        }
        else if (tempMap["type"] == "3")
        {
            pinfo->AiFlag(1);
        }
        else
        {
            pinfo->AiFlag(0);
        }
    }

    gSessionService->UserInfo(pinfo);
}

void LoginServiceImpl::CurrentLoginState(ELOGINSTATUS val)
{
    if (m_currentLoginState == val)
    {
        return;
    }
    if (val == E_LOGIN_ALLOK)
    {
        gSessionService->OKLoginTag(true);
    }
    ET(QString("Q_EMIT S_LoginStateChanged(%1, %2)").arg(m_currentLoginState).arg(val));
    Q_EMIT S_LoginStateChanged(m_currentLoginState, val);
    m_currentLoginState = val;
}

ELOGINSTATUS LoginServiceImpl::CurrentLoginState() const
{
    return m_currentLoginState;
}

void LoginServiceImpl::R_VMMessageParsingFailed()
{
    GERRORLOG("协议解析错误。程序退出。");

    if (CurrentLoginState() == E_LOGIN_RETRY_MAINTCP)
    {
        // do noting.
    }
    else
    {
        Q_ASSERT(false);
        CurrentLoginState(E_LOGIN_RETRY_MONITORTCP);
    }
}

void LoginServiceImpl::R_VM_LoginFinished(QString body)
{
    CurrentLoginState(E_LOGIN_ALLOK);

    gVisitorMonitorTcpManager->LoginOk();

    Q_EMIT S_VM_LoginEnd();

    //访客监控tcp登录成功。
 //   m_protocol->DoExecVMGetVisitorList();
    InitTimer();
}

void LoginServiceImpl::InitTimer()
{
    CreateVisitorMonitorTimer();
}

void LoginServiceImpl::CreateVisitorMonitorTimer()
{
    if (m_visitorMonitorTimer == NULL)
    {
        m_visitorMonitorTimer = new QTimer;
        connect(m_visitorMonitorTimer, SIGNAL(timeout()), this, SLOT(R_OnVisitorMonitorTimer()));
        QString isAutoReply = GetSystemConfigByKey("customeraitype", "0");
        if (isAutoReply == "1")
        {
            m_visitorMonitorTimer->setInterval(30 * 60 * 1000);
        }
        else
        {
            m_visitorMonitorTimer->setInterval(5 * 60 * 1000);
        }
        m_visitorMonitorTimer->start();
    }
    else
    {
        QString isAutoReply = GetSystemConfigByKey("customeraitype", "0");
        if (isAutoReply == "1")
        {
            m_visitorMonitorTimer->setInterval(30 * 60 * 1000);
        }
        else
        {
            m_visitorMonitorTimer->setInterval(5 * 60 * 1000);
        }
        m_visitorMonitorTimer->start();
    }
}

void LoginServiceImpl::R_OnVisitorMonitorTimer()
{
    ;//m_protocol->DoExecVMGetVisitorList();
}

void LoginServiceImpl::R_VM_AuthFinished(bool isOk, int code)
{
    if (isOk)
    {
        UIUserInfo *userinfo = gSessionService->UserInfo();
        m_protocol->DoExecVMLogin(userinfo->UserId(), userinfo->CompanyId());
    }
}

void LoginServiceImpl::R_ForceLogout()
{
    gTcpManager->DisconnectTcp();
    gVisitorMonitorTcpManager->DisconnectTcp();
    CurrentLoginState(E_LOGIN_NONE);
    Q_EMIT S_ForceLogout();
}


void LoginServiceImpl::CreateReloginTimer()
{
    if (m_reLoginTimer == NULL)
    {
        m_reLoginTimer = new QTimer;
        connect(m_reLoginTimer, SIGNAL(timeout()), this, SLOT(R_OnReLoginTime()));

        m_reLoginTimer->setInterval(250);

        m_reLoginTimer->start();
    }
    else
    {
        m_reLoginTimer->setInterval(250);
        m_reLoginTimer->start();
    }
}

void LoginServiceImpl::R_OnReLoginTime()
{
    if (m_reLoginTimer)
    {
        m_reLoginTimer->stop();
    }
    Q_EMIT S_Relogin();
}

void LoginServiceImpl::DoLogin()
{
    m_main->Status(E_LOGIN_NONE);

    Q_EMIT S_AdminAuthLogin();
}

int LoginServiceImpl::GetLoginErrorCount()
{
    return m_loginErrorCount;
}

LoginServerSignal * LoginServiceImpl::LoginSignal()
{
    return m_logingServerSignal;
}
