#include "IAgent.h"
#include "ConstStringHelper.h"
#include "FBDataCenterAgent.h"
#include "KillProcessHelper.h"
#include "LogConsoleAgent.h"
#include "OutdataConsoleAgent.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif
IAgent::IAgent() : m_process(NULL), m_type("")
{
}

IAgent::~IAgent()
{
    ClearProcess();
}

IAgent *IAgent::Instance(QString type, QStringList args, QString cmd)
{
    if (type == ConstStringHelper::EasyLiao)
    {
        return FBDataCenterAgent::Start(args, cmd);
		
	}
	else if (type == ConstStringHelper::Outdata_Console)
	{
		return OutdataConsoleAgent::Start(args);
    }
    else if (type == ConstStringHelper::Log_Console)
     {
            return LogConsoleAgent::Start(args);
     }
        else
    {
        Q_ASSERT(false);
        return NULL;
    }
    return NULL;
}

void IAgent::ClearProcess()
{
    if (m_process)
    {
        m_process->disconnect(this);
        if (m_process->pid())
        {
            
            KillProcessHelper::DoExit(m_process->pid(), 0);
        }
        m_process->deleteLater();
    }
    m_process = NULL;
}

void IAgent::Received(QString &data)
{
    //收到内容，需要做转发功能
    // YGLOG::qLOG->Log4Message("IAgent","IAgent::Received in",YGLOG::Log4Qt::LOG_VERY_DETAIL);
    data = data.trimmed();
    QRegExp reg("^Sender\\s*:(.*);\\s*Receiver\\s*:(.*);\\s*Body\\s*:(.*);");
    if (reg.indexIn(data) != -1)
    {
        QString Sender   = reg.cap(1).trimmed();
        QString Receiver = reg.cap(2).trimmed();
        QString Body     = reg.cap(3).trimmed();
        Q_EMIT DoReceive(Sender, Receiver, Body);
    }
}

void IAgent::Execute(QString dt)
{
    QString info = QString("IAgent::Execute type :%1\r\nbody:\r\n%2\r\n").arg(GetType()).arg(dt);
    // YGLOG::qLOG->Log4Message("RM_Send", info);

    if (m_process)
    {
        m_process->write(dt.toUtf8());
        m_process->write("\n");
        m_process->waitForBytesWritten();

        for (int i = 0; i < 5; i++)
        {
            m_process->write("Test: Message Pump Message Pump\n");
            m_process->waitForBytesWritten();
        }
        //        GPROTOCOLLOG(info, "RM_Send");
    }
}

void IAgent::DoQuit()
{
    if (m_process)
    {
        if (m_process->pid())
        {
            KillProcessHelper::DoExit(m_process->pid(), 0);
        }
    }
}

void   IAgent::StartProcess()
{
	

}
