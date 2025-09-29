#include "FBDataCenterAgent.h"
#include "ConstStringHelper.h"

FBDataCenterAgent::FBDataCenterAgent()
{
    m_type = ConstStringHelper::EasyLiao;
}

FBDataCenterAgent::~FBDataCenterAgent()
{
}

FBDataCenterAgent *FBDataCenterAgent::Start(QStringList args, QString cmd)
{
    args.push_back(cmd);

    auto fbDateCenterAgent       = new FBDataCenterAgent();
    fbDateCenterAgent->m_process = new QProcess(fbDateCenterAgent);
    fbDateCenterAgent->m_process->setProcessChannelMode(QProcess::MergedChannels);

    connect(fbDateCenterAgent->m_process, SIGNAL(readyReadStandardOutput()), fbDateCenterAgent, SLOT(Output()));
    connect(fbDateCenterAgent->m_process, SIGNAL(finished(int, QProcess::ExitStatus)), fbDateCenterAgent, SLOT(Finished(int, QProcess::ExitStatus)));
    connect(fbDateCenterAgent->m_process, SIGNAL(readyReadStandardError()), fbDateCenterAgent, SLOT(StandardError()));
    fbDateCenterAgent->m_process->start("EasyLiaoClient.exe", args, QIODevice::Unbuffered | QIODevice::ReadWrite);

    auto isStart = fbDateCenterAgent->m_process->waitForStarted();
    if (!isStart)
    {
        delete fbDateCenterAgent;
        return NULL;
    }
    return fbDateCenterAgent;
}

void FBDataCenterAgent::Finished(int exitCode, QProcess::ExitStatus status)
{
    if (status == QProcess::CrashExit)
    {
        // crash exit
        Q_EMIT DoExit(exitCode, true);
    }
    else
    {
        //正常退出
        Q_EMIT DoExit(exitCode, false);
    }
}

void FBDataCenterAgent::Output()
{
    while (m_process != NULL && m_process->canReadLine())
    {
        QString line = QString::fromUtf8(m_process->readLine());
        if (line.startsWith(ConstStringHelper::Troy))
        {
            line = line.remove(0, ConstStringHelper::Troy.length()).toUtf8();
            Received(line);
        }
    }
}

void FBDataCenterAgent::StandardError()
{
    while (m_process != NULL && m_process->canReadLine())
    {
        auto b64 = QString::fromUtf8(m_process->readLine());
        do
        {
            if (b64.startsWith(ConstStringHelper::Troy))
            {
                auto data = QString::fromUtf8(QByteArray::fromBase64(b64.toUtf8()));
                //                GERRORLOG(data);
            }
        } while (false);
    }
}
