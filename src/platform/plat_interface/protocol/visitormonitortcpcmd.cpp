#include "VisitorMonitorTcpCmd.h"
#include "protocol/platprotocolservice.h"


VisitorMonitorTcpCmd::VisitorMonitorTcpCmd() : ICmd()
{
    m_hasResponse = false;
}

VisitorMonitorTcpCmd::~VisitorMonitorTcpCmd()
{
}

void VisitorMonitorTcpCmd::SendRequestToServer()
{
    TcpMessagePtr obj = gVisitorMonitorTcpManager->CreateNewResquest();

    connect(obj.data(), SIGNAL(S_SendMessageFinished(TcpMessagePtr)), this, SLOT(R_Finished(TcpMessagePtr)));
    obj->Body(Body());
    obj->Uri(Uri());
    obj->HasResponse(HasResponse());
    for (auto begin = m_headerList.begin(); begin != m_headerList.end(); begin++)
    {
        obj->AddHeader(begin.key(), begin.value());
    }
    gVisitorMonitorTcpManager->SendTcpMessage(obj);
}

void VisitorMonitorTcpCmd::MakeCmd()
{
    Body("OK");
    Uri("");
    HasResponse(false);
}

void VisitorMonitorTcpCmd::PreProcessMsg(TcpMessagePtr msg)
{

    Q_EMIT S_Finished(msg);
}

void VisitorMonitorTcpCmd::AddHeader(QString key, QString val)
{
    m_headerList.insert(key, val);
}

QString VisitorMonitorTcpCmd::GetHeaderByKey(QString key)
{
    if (m_headerList.find(key) != m_headerList.end())
    {
        return m_headerList[key];
    }
    return "";
}

void VisitorMonitorTcpCmd::R_Finished(TcpMessagePtr msg)
{
    PreProcessMsg(msg);
}
