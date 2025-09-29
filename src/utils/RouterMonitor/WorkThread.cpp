#include "WorkThread.h"


WorkThread::WorkThread(): QThread()
{
	m_agent = nullptr;
}


void WorkThread::run()
{
	if (m_agent)
		m_agent->StartProcess();
    exec();
}
