#include "utils/EL_Util_Time.h"

#include "ProtocolLoginWork.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QStyle>
#include <QStyleFactory>


#include "data/CommInfo.h"
#include "data/LicenceSetting.h"
#include "data/PermissionsSetting.h"

#include "utils/DefineHelper.h"
#include "protocol_biz/protocol_login_agent.h"
#include "protocol_login_agentImp.h"

#include "utils/utilshelper.h"

#include "./utils/utilshelper.h"
#include "utils/ConstStringHelper.h"
#include "data/Im_Message.h"
#include "service/dbuiservice.h"
#include "thread/CheckThread.h"

#include "protocol/tcpmanager.h"
#include "protocol/visitormonitortcpmanager.h"
#include "service/util_service.h"
#include "protocol/TcpMessage.h"
#include "protocol/RequestKeepLive.h"
#include "tcp/elconnectState.h"

ProtocolLoginWork::ProtocolLoginWork() : QObject()
{
    m_firstGetCurrentChat      = true;
    m_currentLoginState        = E_LOGIN_NONE;

    m_offlinecount             = 0;
    m_currentChatEnd           = true;
	m_dispathThread = nullptr;

    m_factoryBaseData = new BaseConstructionData();
    // qRegisterMetaType< UICardInfo >("UICardInfo");
    qRegisterMetaType<CommInfo>("CommInfo");
    qRegisterMetaType<QList<CommInfo *>>("QList<CommInfo*>");
    qRegisterMetaType<ELOGINSTATUS>("ELOGINSTATUS");
    qRegisterMetaType<Im_Message>("Im_Message");
    qRegisterMetaType<EVISITORSTATUS>("EVISITORSTATUS");
    qRegisterMetaType<EPRESENCESTATUS>("EPRESENCESTATUS");
    qRegisterMetaType<EUSERTYPE>("EUSERTYPE");
    qRegisterMetaType<EMESSAGETYPE>("EMESSAGETYPE");
    qRegisterMetaType<ELTCPlLOGINSTATUS>("ELTCPlLOGINSTATUS");

    qRegisterMetaType<EURITYPE>("EURITYPE");
    
	qRegisterMetaType<QSharedPointer<TcpMessage>>("QSharedPointer<TcpMessage>");
	qRegisterMetaType<TcpMessage *>("TcpMessage *");
	qRegisterMetaType<TcpMessagePtr>("TcpMessagePtr");

	connect(this, SIGNAL(S_ThreadInit()), this, SLOT(R_ThreadInit()));

    m_exitclose = false;
}

ProtocolLoginWork::~ProtocolLoginWork()
{
}

void ProtocolLoginWork::Init()
{
//   connect(m_protocol, SIGNAL(S_GetInitDataFinished(QString, QString, QString, QString)), this, SLOT(R_GetInitDataFinished(QString, QString, QString, QString)));

    connect(this, SIGNAL(S_ForceLogout()),m_protocol , SIGNAL(S_ForceLogout()));
    connect(this, SIGNAL(S_ForceReLogin(int)), m_protocol, SIGNAL(S_ForceReLogin(int)));

  connect(this, SIGNAL(S_VM_LoginEnd()), m_protocol, SIGNAL(S_VM_LoginEnd()));
    connect(this, SIGNAL(S_AddMonitorVisitorList()), m_protocol, SIGNAL(S_AddMonitorVisitorList()));

       connect(this, SIGNAL(S_Received_KeepAlive_State(EPRESENCESTATUS)), m_protocol, SIGNAL(S_Received_KeepAlive_State(EPRESENCESTATUS)));
     connect(this, SIGNAL(S_HeartPaceTimeOut()), m_protocol, SIGNAL(S_HeartPaceTimeOut()));


	 connect(this, SIGNAL(S_SendTcpMessage(QByteArray)), gTcpHvManager, SIGNAL(S_SendTcpMessage(QByteArray)));
	 
	 
}

void ProtocolLoginWork::onInitTimer()
{
    m_exitclose      = true;
    m_heartPaceTimer = new QTimer(this);
    connect(m_heartPaceTimer, SIGNAL(timeout()), this, SLOT(onHeartPaceTimeOut()));
    m_heartPaceTimer->start(ConstStringHelper::HeartPace);
}

void ProtocolLoginWork::Protocol(IProtocolLoginAgentImp *imp)
{
    m_protocol = imp;
}

void ProtocolLoginWork::InitTimer()
{

}

void ProtocolLoginWork::Clear()
{
}

void ProtocolLoginWork::SetUserState(EPRESENCESTATUS state)
{
    EPRESENCESTATUS oldState = m_curPresenceStatus;
    m_curPresenceStatus      = state;
    int val                  = 1;
    switch (state)
    {
        case E_ONLINE:
            val = 1;
            break;
        case E_BUSY:
            val = 2;
            break;
        case E_LEAVE:
            val = 3;
            break;
        default:
            Q_ASSERT(false);
            GERRORLOG("设置状态出错。不该有当前状态。");
            break;
    }
/*
    gIProtocolLoginAgent->DoExecSetUserStateCmd(val);

    //保存状态。
    UILoginData *loginData = gSessionService->LoginData();
    if ((loginData) && (!loginData->UserName().isEmpty()))
    {
        loginData->UserState(val - 1);

        gSessionService->LoginData(loginData);
        gUISystemConfigManager->SaveLocalSetting();
    }
    if ((gSessionService->UserInfo()) && (!gSessionService->UserInfo()->UserId().isEmpty()))
    {
        gUIColleagueManager->SetPresenceStatus(gSessionService->UserInfo()->UserId(), state);
        Q_EMIT S_CustomerPresenceChanged(gSessionService->UserInfo()->UserId(), state, oldState);
    }
    */
}



void ProtocolLoginWork::R_UploadDumpFile()
{
    /*
    QString path = UtilsHelper::GetDumpDir();
    QDir    dir(path);
    if (!dir.exists())
    {
        return;
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list       = dir.entryInfoList();
    int           file_count = list.count();
    if (file_count <= 0)
    {
        return;
    }

    //产生备份目录
    UtilsHelper::GetDumpBakDir();
    QStringList string_list;
    for (int i = 0; i < file_count; i++)
    {
        QFileInfo file_info = list.at(i);
        QString   suffix    = file_info.suffix();
        if (QString::compare(suffix, QString("dmp"), Qt::CaseInsensitive) == 0)
        {
            QString absolute_file_path = file_info.absoluteFilePath();
            string_list.append(absolute_file_path);
        }
    }

    if (gSessionService->UserInfo())
    {
        foreach (QString fileName, string_list)
        {
            //            minidump_公司id_帐号id_版本_日期-时间
            QString bakfilename = path + "minidump_";
            bakfilename += gSessionService->UserInfo()->CompanyId();
            bakfilename += "_";
            bakfilename += gSessionService->UserInfo()->UserId();
            bakfilename += "_";
            bakfilename += gUISystemConfigManager->GetCurrentVersion();
            bakfilename += "_";
            QFileInfo info(fileName);
            if (info.exists())
            {
                bakfilename += info.created().toString("yyyyMMdd-hhmmss");
            }
            bakfilename += ".dmp";

            QFile file(fileName);
            if (file.exists())
            {
                if (file.rename(bakfilename))
                {
                    m_protocol->DoUploadDumpFile(bakfilename);
                }
            }
        }
    }
    */
}


void ProtocolLoginWork::R_OnParentHeartPaceTimeOut()
{
    /*	UIUserInfo* currentUser = gSessionService->UserInfo();
    if ((currentUser == nullptr)||((currentUser->CompanyId() != "12514")&&(currentUser->CompanyId() != "10505") ) )*/
    {
        RequestKeepLive req;
        QString         body = req.MakeXml();
        UtilsHelper::Execute(body);
    }
}



void ProtocolLoginWork::onReceiveCmdTask(QString cmd)
{
	//Sender：ddd；Receiver：sss，sss；Body:uuu;
	do
	{
		QRegExp reg("^Sender\\s*:(.*);\\s*Receiver\\s*:(.*);\\s*Body\\s*:(.*)");
		if (reg.indexIn(cmd) != -1)
		{
			QString Sender = reg.cap(1).trimmed();
			QString Receiver = reg.cap(2).trimmed();

			if (Sender.isEmpty())
			{
				break;
			}

			if (Receiver.compare(ConstStringHelper::EasyLiao, Qt::CaseInsensitive) != 0)
			{
				break;
			}

			QString Body = reg.cap(3).trimmed();
			QString data = QString::fromUtf8(QByteArray::fromBase64(Body.toUtf8()));
			QString info = QString("RevProtocol:%1\n").arg(data);

			if (Sender == ConstStringHelper::RouteMonitor)
			{
				QString req;
				int res = ParseProtocol(data, req, Sender);
				/*
				//回包
				ReplyPrototcol(req, Sender, Receiver, res);
				*/
			}
			else if (Sender == ConstStringHelper::WxPhoneOpt)
			{
				//ParseWxPhoneOptProtocol(data);

			}
			else if (Sender == ConstStringHelper::OutdataConsole)
			{
				QString req;
				int res = ParseProtocol(data, req, Sender);


			}
		}
	} while (false);
}



int ProtocolLoginWork::ParseProtocol(const QString &body, QString& req, QString& sender)
{
	int rescode = ConstStringHelper::ECUnknownProtocol;
	do
	{
		QDomDocument doc;
		doc.setContent(body);
		QDomElement docElem = doc.documentElement();
		auto tn = docElem.tagName();

		//模块版本
		if (tn.compare(ConstStringHelper::BizModuleInfo, Qt::CaseInsensitive) == 0)
		{
			rescode = ConstStringHelper::ModleVerCode;
			break;
		}
		
		//心跳
		if (tn.compare(ConstStringHelper::BizKeepAlive, Qt::CaseInsensitive) == 0)
		{
			if (m_heartPaceTimer)
			{
				//m_heartPaceTimer->stop();
				//m_heartPaceTimer->start( ConstStringHelper::ParentHeartPace );
				m_exitclose = false;

			}
			rescode = ConstStringHelper::NoReplyCode;
			break;
		}

		//外部测试数据
		if (tn.compare(ConstStringHelper::BizDemoProcotol, Qt::CaseInsensitive) == 0)
		{
			int type = 0;
			QString msg;
			QDomNodeList childList = docElem.toElement().childNodes();
			for (int i = 0; i < childList.count(); ++i)
			{
				QDomNode childNode = childList.at(i);
				if (childNode.isElement())
				{
					QDomElement itemElem = childNode.toElement();
					auto itemName = itemElem.tagName();
					if (itemName == "type" )
					{
						type = itemElem.text().toInt();	
					}
					else if (itemName == "msg")
					{
						msg = itemElem.text();
					}

				}

			}

			if( !msg.isEmpty() )
			{ 
				switch (type )
				{
				case 1:
					gTcpHvManager->SendTcpMessage(msg.toUtf8());
					break;
				case 2:
					//接收
					
					
					gTcpHvManager->ReceiveOutdataTcpMessage(msg.toUtf8());
					break;
				case 91:
				{
					//崩馈测试
					char * tt = nullptr;
					if (0) tt = (char *)malloc(100);
					strcpy(tt, "asdfasfdasfasdfdasf");
					
				}
					break;
				default:
					break;
				}
			
			}

			break;
		}
	} while (false);

	return rescode;
}

void ProtocolLoginWork::ReplyPrototcol(const QString& req, const QString &sender, const QString &receiver, int rescode)
{

    switch (rescode)
	{
	case ConstStringHelper::NoReplyCode:
		break;
	case ConstStringHelper::ModleVerCode:
	{
		
	}
	break;
	default:
	{
	
	}
	break;
	}
}


void ProtocolLoginWork::R_ThreadInit()
{
	if (m_dispathThread == nullptr)
	{
		m_dispathThread = new DispathThread;
		//m_checkThread = new CheckThread(this);
		connect(m_dispathThread, SIGNAL(ReceiveCmdData(QString)), this, SLOT(onReceiveCmdTask(const QString &)));


		//connect(m_dispathThread, SIGNAL(InitTimer()), this, SLOT(onInitTimer()));
		m_dispathThread->start();
		m_parentHeartPaceTimer = new QTimer(this);
		connect(m_parentHeartPaceTimer, SIGNAL(timeout()), this, SLOT(R_OnParentHeartPaceTimeOut()));
		m_parentHeartPaceTimer->start(ConstStringHelper::ParentHeartPace);
	}
}
