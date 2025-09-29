#include "outdataconsolework.h"
#include "IAgent.h"

OutdataConsoleWork::OutdataConsoleWork(QObject *parent) : QObject(parent)
{

	m_agent = nullptr;
}


void OutdataConsoleWork::R_LogInfo(QString body)
{

	m_agent->Execute(body);

}

void    OutdataConsoleWork::SetAgent(IAgent *agent)
{
	m_agent = agent;
}