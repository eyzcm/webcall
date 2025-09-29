#include "logconsolework.h"
#include "IAgent.h"

LogConsoleWork::LogConsoleWork(QObject *parent) : QObject(parent)
{

	m_agent = nullptr;
}


void LogConsoleWork::R_LogInfo(QString body)
{

	m_agent->Execute(body);

}

void    LogConsoleWork::SetAgent(IAgent *agent)
{
	m_agent = agent;
}