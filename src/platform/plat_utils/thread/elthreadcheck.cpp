#include "elthreadcheck.h"
//#include "./protocol_biz/protocol_agentImp.h"


ElThreadCheck::ElThreadCheck() : QThread()
{
	m_mainobj = nullptr;
}

ElThreadCheck::~ElThreadCheck()
{
}

void ElThreadCheck::setThreadName(QString name)
{
    m_threadname = name;
}

void ElThreadCheck::InitConnect()
{
	if (m_mainobj)
	{
		Q_EMIT  S_ThreadInit();
	}
    /*
    if (IProtocolAgentImp::GetInstance()->WorkObject())
        connect(IProtocolAgentImp::GetInstance()->WorkObject()->checkThread(), SIGNAL(S_RunCheck()), this, SLOT(R_RunCheck()));
        */
}

void ElThreadCheck::R_RunCheck()
{
    /*
    if (IProtocolAgentImp::GetInstance()->WorkObject())
        IProtocolAgentImp::GetInstance()->WorkObject()->checkThread()->SetTimer(m_threadname, GetTickCount());
        */
}

void ElThreadCheck::SetMainObject(QObject * mainobj)
{
	if (m_mainobj) disconnect(this, SIGNAL(S_ThreadInit()), this, SIGNAL(S_ThreadInit()));;
	m_mainobj = mainobj;
	connect(this, SIGNAL(S_ThreadInit()), m_mainobj, SIGNAL(S_ThreadInit()));

}
