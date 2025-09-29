#include "LogConsoleAgent.h"
#include "ConstStringHelper.h"

LogConsoleAgent::LogConsoleAgent()
{
    m_type = ConstStringHelper::Log_Console;
}

LogConsoleAgent::~LogConsoleAgent()
{
}

LogConsoleAgent* LogConsoleAgent::Start(QStringList args)
{
	auto agentObj = new LogConsoleAgent();
#if 0
	agentObj->m_process = new QProcess( agentObj );
    agentObj->m_process->setProcessChannelMode(QProcess::MergedChannels);

	connect( agentObj->m_process, SIGNAL( readyReadStandardOutput() ),
                        agentObj,   SLOT( Output() ) );
	connect( agentObj->m_process, SIGNAL( finished( int, QProcess::ExitStatus ) ),
                        agentObj,   SLOT( Finished( int, QProcess::ExitStatus ) ) );
	connect( agentObj->m_process, SIGNAL( readyReadStandardError() ),
                        agentObj,   SLOT( StandardError() ) );

#if _DEBUG
	agentObj->m_process->start( "log_console.exe", args, QIODevice::Unbuffered|QIODevice::ReadWrite );
#else
	agentObj->m_process->start( "log_console.exe", args,  QIODevice::Unbuffered|QIODevice::ReadWrite );
#endif

	auto isStart = agentObj->m_process->waitForStarted();
	if ( !isStart )
	{
		delete agentObj;
		return NULL;
	}
#endif

	return agentObj;
}

void   LogConsoleAgent::StartProcess()
{
	if (this->m_process == nullptr)
	{
		QStringList args;
		args.append("log");

		this->m_process = new QProcess(this);
		this->m_process->setProcessChannelMode(QProcess::MergedChannels);

		connect(this->m_process, SIGNAL(readyReadStandardOutput()),
			this, SLOT(Output()));
		connect(this->m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(Finished(int, QProcess::ExitStatus)));
		connect(this->m_process, SIGNAL(readyReadStandardError()),
			this, SLOT(StandardError()));

#if _DEBUG
		this->m_process->start("easyliaoclient.exe", args, QIODevice::Unbuffered | QIODevice::ReadWrite);
#else
		this->m_process->start("easyliaoclient.exe", args, QIODevice::Unbuffered | QIODevice::ReadWrite);
#endif

		auto isStart = this->m_process->waitForStarted();
		if (!isStart)
		{
			this->m_process = nullptr;
			//delete agentObj;

		}
	}
}

void LogConsoleAgent::Finished( int exitCode, QProcess::ExitStatus status )
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

void LogConsoleAgent::Output()
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

void LogConsoleAgent::StandardError()
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

