#include "RMAgent.h"
#include "ConstStringHelper.h"
#include "IAgent.h"
#include "KillProcessHelper.h"
#include "RequestKeepLive.h"
#include "RequestReTryProcess.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#ifdef Q_OS_WIN32
#include <windows.h>
#endif
#include <QMetaType>

#define StringCompare(x, y) x.compare(y, Qt::CaseInsensitive) == 0

RMAgent::RMAgent() : m_dataCenterAgent(NULL), m_req(1), m_timer(NULL)
{
    m_subtimer        = nullptr;
    m_username        = "";
    m_password        = "";
    m_servers         = "im.easyliao.com";
    m_port            = "1890";
    m_dataCenterAgent = nullptr;
    m_logConsoleAgent = nullptr;
	m_outdataAgent = nullptr;
    m_nCount          = 1;
}

RMAgent::~RMAgent()
{
    if (m_timer)
    {
        m_timer->stop();
    }
    if (m_subtimer)
    {
        m_subtimer->stop();
    }
    if (m_dataCenterAgent)
    {
        m_dataCenterAgent->disconnect(this);
        m_dataCenterAgent->ClearProcess();
    }
}

void RMAgent::start(QString cmd)
{
    if (!CreateDataCenterAgent(false, cmd))
    {
        return;
    }
    CreateTimer();

    CreateConsoleAgent(false);
	CreateOutDataAgent(false);
}

void RMAgent::KeepAlive()
{
    RequestKeepLive p;
    QString         body = p.MakeXml();
    body                 = body.toUtf8().toBase64();
    QString result       = "";
    if (m_dataCenterAgent)
    {
        result = ConstStringHelper::ProtocolFormat.arg(ConstStringHelper::RouteMonitor).arg(ConstStringHelper::EasyLiao).arg(body);
        m_dataCenterAgent->Execute(result);
    }
}

void RMAgent::DoReceive(QString sender, QString receiver, QString body)
{
    QString result = ConstStringHelper::ProtocolFormat.arg(sender).arg(receiver).arg(body);

    QString info = QString("RMAgent::DoReceive\r\n sender:%1 receiver:%2 body:\r\n%3").arg(sender).arg(receiver).arg(QString(QByteArray::fromBase64(body.toUtf8())));

    //    GLOG(info);
    if (StringCompare(receiver, ConstStringHelper::RouteMonitor))
    {
        QString data = QString::fromUtf8(QByteArray::fromBase64(body.toUtf8()));
        PraseProtocol(sender, data.toUtf8());
    }
    else if (StringCompare(receiver, ConstStringHelper::EasyLiao))
    {
        if (m_dataCenterAgent)
        {
            m_dataCenterAgent->Execute(result);
        }
	}
    else if (StringCompare(receiver, ConstStringHelper::Log_Console))
    {
		//日志信息
		emit S_LogInfo(result);
    }
    else
    {
        if (m_dataCenterAgent)
        {
            m_dataCenterAgent->Execute(result);
        }
    }
}

void RMAgent::PraseProtocol(const QString &sender, const QString &body)
{
    QString info = QString("RMAgent::PraseProtocol \r\n%1").arg(body);
    QString type = GetProtocolType(body);
    if (StringCompare(type, ConstStringHelper::BizExitLogin))
    {
        RunExit(125);
    }
    else if (StringCompare(type, ConstStringHelper::BizLogoff))
    {
        Do_LogoffClient_Protocol();
    }
    else if (StringCompare(type, ConstStringHelper::BizResponse))
    {
        Do_Response_Protocol(body);
    }
    else if (StringCompare(type, "bizlogininfo"))
    {
        Do_SetLoginInfo(body);
		QString data = body.toUtf8().toBase64();;
		QString result = ConstStringHelper::ProtocolFormat.arg(ConstStringHelper::RouteMonitor).arg(ConstStringHelper::Outdata_Console).arg(data);
		m_outdataAgent->Execute(result);
    }
    else if (StringCompare(type, "bizlogoutinfo"))
    {
        Do_LogoutInfo(body);
    }
    else if (StringCompare(type, ConstStringHelper::BizKeepAlive))
    {
        if (StringCompare(sender, ConstStringHelper::EasyLiao))
        {
            Do_KeepAlive(body);
        }

	
  
    }
    else
    {
        Q_ASSERT(false);
    }
}

void RMAgent::DoExit(int ncode, bool isCrash)
{
    QString val = QString("RMAgent::DoExit code:  %1").arg(ncode);
    //    GERRORLOG(val);
    if (m_dataCenterAgent == sender())
    {
        if (m_subtimer)
        {
            m_subtimer->stop();
        }
        if (isCrash)
        {
            Q_ASSERT(false);
            //退出进程
            if (!CreateDataCenterAgent(true))
            {
                return;
            }
        }
        else
        {
            if (ncode == 0)
            {
                //退出进程
                RunExit(ncode, true);
                return;
            }
            else if (ncode == 1073807364)
            {
                RunExit(ncode, true);
                return;
            }
            else
            {
                if (!CreateDataCenterAgent(true))
                {
                    Q_ASSERT(false);
                    return;
                }
            }
        }
    }
    else
    {
        Q_ASSERT(false);
    }
    QString error = QString("RMAgent::Exit ncode:%1 isCrash:%2").arg(ncode).arg(isCrash);
    //    GERRORLOG(error);
}

static RMAgent *m_instance = NULL;
RMAgent        *RMAgent::instance()
{
    if (!m_instance)
        m_instance = new RMAgent();
    return m_instance;
}

void RMAgent::Do_LogoffClient_Protocol()
{
    if (m_dataCenterAgent)
    {
        m_dataCenterAgent->disconnect(this);
        m_dataCenterAgent->ClearProcess();
        if (!CreateDataCenterAgentLoginOff())
        {
            return;
        }
    }
}

void RMAgent::RunExit(int ncode, bool isForce)
{
    if (isForce)
    {
        //删除目录
        if (m_dataCenterAgent)
        {
            m_dataCenterAgent->disconnect(this);
            m_dataCenterAgent->ClearProcess();
            m_dataCenterAgent->deleteLater();
            m_dataCenterAgent = NULL;
        }
        else
        {
            QString val = QString("RMAgent::RunExit m_dataCenterAgent is null    code:  %1\r\n").arg(ncode);
            //            GERRORLOG(val);
        }
		if (m_logConsoleAgent)
		{
			m_logConsoleAgent->disconnect(this);
			m_logConsoleAgent->ClearProcess();
			m_logConsoleAgent->deleteLater();
			m_logConsoleAgent = NULL;
		}
		if (m_outdataAgent)
		{
			m_outdataAgent->disconnect(this);
			m_outdataAgent->ClearProcess();
			m_outdataAgent->deleteLater();
			m_outdataAgent = NULL;

		}
#ifdef Q_OS_WIN
        Q_PID pid;
        pid->dwProcessId = QCoreApplication::applicationPid();
           KillProcessHelper::DoExit( pid, 0);
#else
        KillProcessHelper::DoExit(QCoreApplication::applicationPid(), 0);
#endif
    }
    else
    {
        if (m_dataCenterAgent)
        {
            m_dataCenterAgent->disconnect(this);
            m_dataCenterAgent->ClearProcess();
            m_dataCenterAgent->deleteLater();
            m_dataCenterAgent = NULL;
        }
        else
        {
            QString val = QString("RMAgent::RunExit m_dataCenterAgent is null    code:  %1\r\n").arg(ncode);
            //            GERRORLOG(val);
        }
		if (m_logConsoleAgent)
		{
			m_logConsoleAgent->disconnect(this);
			m_logConsoleAgent->ClearProcess();
			m_logConsoleAgent->deleteLater();
			m_logConsoleAgent = NULL;
		}
		if (m_outdataAgent)
		{
			m_outdataAgent->disconnect(this);
			m_outdataAgent->ClearProcess();
			m_outdataAgent->deleteLater();
			m_outdataAgent = NULL;
		}

#ifdef Q_OS_WIN
        Q_PID pid;
        pid->dwProcessId = QCoreApplication::applicationPid();
           KillProcessHelper::DoExit( pid, 0);
#else
        KillProcessHelper::DoExit(QCoreApplication::applicationPid(), 0);
#endif
        QString val = QString("RMAgent::RunExit  m_dataCenterAgent is not null  code:  %1\r\n").arg(ncode);
        //        GERRORLOG(val);
    }





}


bool RMAgent::CreateDataCenterAgent(bool isRetry, QString cmd)
{
    bool        isRet = true;
    QStringList arglist;
	arglist.push_back("client");
    if (!m_username.isEmpty() && !m_password.isEmpty())
    {
        arglist.push_back(m_username);
        arglist.push_back(m_password);
        arglist.push_back(m_servers);
        arglist.push_back(m_port);
    }

    m_dataCenterAgent = IAgent::Instance(ConstStringHelper::EasyLiao, arglist, cmd);
    if (m_dataCenterAgent)
    {
        connect(m_dataCenterAgent, SIGNAL(DoExit(int, bool)), this, SLOT(DoExit(int, bool)));
        connect(m_dataCenterAgent, SIGNAL(DoReceive(QString, QString, QString)), this, SLOT(DoReceive(QString, QString, QString)));
    }
    else
    {
        isRet = false;
        RunExit(122);
    }
    return isRet;
}

void RMAgent::CreateTimer()
{
    if (m_timer == nullptr)
    {
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(KeepAlive()));
    }
    m_timer->start(50 * 1000);

    if (m_subtimer == nullptr)
    {
        m_subtimer = new QTimer(this);
        connect(m_subtimer, SIGNAL(timeout()), this, SLOT(SubKeepAlive()));
    }
    m_subtimer->start(150 * 1000);
}

QString RMAgent::GetProtocolType(const QString &body)
{
    QDomDocument doc;
    doc.setContent(body);
    QDomElement docElem = doc.documentElement();
    QString     type    = docElem.tagName().trimmed();
    return type;
}

void RMAgent::Do_Response_Protocol(const QString &body)
{
    QDomDocument doc;
    doc.setContent(body);
    QDomElement docElem = doc.documentElement();

    QString req = docElem.attribute(ConstStringHelper::req);
    if (!req.isEmpty())
    {
        IRequest *obj = m_requestMap[req];
        if (obj)
        {
            obj->deleteLater();
            obj = NULL;
        }
    }
}

void RMAgent::Do_SetLoginInfo(const QString &body)
{
    if (m_subtimer)
    {
        m_subtimer->start(150 * 1000);
    }
    QDomDocument doc;
    doc.setContent(body);
    QDomElement docElem = doc.documentElement();
    auto        m       = Convert2Map(docElem);

    m_username = m["username"];
    m_password = m["password"];
    m_servers  = m["servers"];
    m_port     = m["port"];
}

QMap<QString, QString> RMAgent::Convert2Map(const QDomElement &ele)
{
    QDomNode               n = ele.firstChild();
    QMap<QString, QString> valuemap;
    while (!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if (!e.isNull())
        {
            QString strTagName   = e.tagName().trimmed();
            QString strValue     = e.text().trimmed();
            valuemap[strTagName] = strValue;
        }
        n = n.nextSibling();
    }

    return valuemap;
}

void RMAgent::Do_KeepAlive(const QString &body)
{
    m_nCount++;
}

void RMAgent::Do_LogInfo(const QString &body)
{
  
	emit S_LogInfo(body);

}

void RMAgent::SubKeepAlive()
{
    if (m_nCount == 0)
    {
        QString val = QString("RMAgent::SubKeepAlive\r\n");
        //        GLOG(val);
        if (!CreateDataCenterAgent(true))
        {
            return;
        }
    }
    else
    {
        m_nCount = 0;
    }
}

void RMAgent::Do_LogoutInfo(const QString &body)
{
    QString val = QString("RMAgent::Do_LogoutInfo\r\n");
    //    GPROTOCOLLOG(val, "Do_LogoutInfo");
    RunExit(0, false);
}

bool RMAgent::CreateDataCenterAgentLoginOff()
{
    bool        isRet = true;
    QStringList arglist;

    m_dataCenterAgent = IAgent::Instance(ConstStringHelper::EasyLiao, arglist, "logout");
    if (m_dataCenterAgent)
    {
        connect(m_dataCenterAgent, SIGNAL(DoExit(int, bool)), this, SLOT(DoExit(int, bool)));
        connect(m_dataCenterAgent, SIGNAL(DoReceive(QString, QString, QString)), this, SLOT(DoReceive(QString, QString, QString)));
    }
    else
    {
        isRet = false;
        RunExit(122);
    }
    return isRet;
}


bool RMAgent::CreateOutDataAgent(bool isRetry /*= false*/)
{
	bool isRet = true;
	QStringList arglist;

	m_outdataAgent = IAgent::Instance(ConstStringHelper::Outdata_Console, arglist);
	if (m_outdataAgent)
	{

		connect(m_outdataAgent, SIGNAL(DoReceive(QString, QString, QString)), this, SLOT(DoReceive(QString, QString, QString)));
		connect(this, SIGNAL(S_OutdataInfo(QString)), &m_outdataConsoleWork, SLOT(R_OutdataInfo(QString)));
	}
	else
	{
		isRet = false;
	}
	
	return isRet;
}
bool RMAgent::CreateConsoleAgent(bool isRetry /*= false*/)
{
    bool isRet = true;
    QStringList arglist;

    m_logConsoleAgent = IAgent::Instance(ConstStringHelper::Log_Console, arglist);
    if (m_logConsoleAgent)
    {
        
        connect( m_logConsoleAgent, SIGNAL( DoReceive( QString,QString,QString ) ), this, SLOT( DoReceive( QString,QString,QString )) );
        connect( this,  SIGNAL( S_LogInfo(QString )), &m_logConsoleWork, SLOT( R_LogInfo(QString )));
    }
    else
    {
        isRet = false;
    }
	m_workThread.m_agent = m_logConsoleAgent;
	m_logConsoleWork.SetAgent( m_logConsoleAgent );
    m_logConsoleWork.moveToThread( &m_workThread );
    m_workThread.start();
    return isRet;
}

