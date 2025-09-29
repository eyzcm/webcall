#include "OutdataConsoleAgent.h"
#include "ConstStringHelper.h"

OutdataConsoleAgent::OutdataConsoleAgent()
{
    m_type = ConstStringHelper::Outdata_Console;
}

OutdataConsoleAgent::~OutdataConsoleAgent()
{
}

OutdataConsoleAgent* OutdataConsoleAgent::Start(QStringList args)
{
	auto agentObj = new OutdataConsoleAgent();
	agentObj->StartProcess();
	return agentObj;
}

void   OutdataConsoleAgent::StartProcess()
{
	if (this->m_process == nullptr)
	{
		QStringList args;
		args.append("dataprotocol");

		this->m_process = new QProcess(this);
		this->m_process->setProcessChannelMode(QProcess::MergedChannels);

		connect(this->m_process, SIGNAL(readyReadStandardOutput()),
			this, SLOT(Output()));
		connect(this->m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(Finished(int, QProcess::ExitStatus)));
		connect(this->m_process, SIGNAL(readyReadStandardError()),
			this, SLOT(StandardError()));

#if _DEBUG
        this->m_process->start("easyliaoclienttest.exe", args, QIODevice::Unbuffered | QIODevice::ReadWrite);
#else
        this->m_process->start("easyliaoclienttest.exe", args, QIODevice::Unbuffered | QIODevice::ReadWrite);
#endif

		auto isStart = this->m_process->waitForStarted();
		if (!isStart)
		{
			this->m_process = nullptr;
			//delete agentObj;

		}
	}
}

void OutdataConsoleAgent::Finished( int exitCode, QProcess::ExitStatus status )
{
	if ( status == QProcess::CrashExit )
	{
		//crash exit
		emit DoExit(exitCode,true);
	}
	else
	{
		//正常退出
		emit DoExit(exitCode,false);	
	}
}

void OutdataConsoleAgent::Output()
{
	while (m_process !=NULL && m_process->canReadLine() )
	{
		QString line = QString::fromUtf8( m_process->readLine() );
		if ( line.startsWith( ConstStringHelper::Troy ) )
		{
			line =  line.remove( 0, ConstStringHelper::Troy.length() ).toUtf8();
			Received( line );
		}
	}
}

void OutdataConsoleAgent::StandardError()
{

	while (m_process!=NULL&& m_process->canReadLine() )
	{
		auto b64 = QString::fromUtf8( m_process->readLine() );
		do
		{
			if (b64.startsWith( ConstStringHelper::Troy))
			{				
				auto data = QString::fromUtf8( QByteArray::fromBase64( b64.toUtf8() ) );
				return;
			}
			else
			{
				break;
			}
		}
		while ( false );		
	}
}

