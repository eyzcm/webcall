#include "tcpcmd.h"
#include "utils/EL_Util_Time.h"
#include "protocol/platprotocolservice.h"

TcpCmd::TcpCmd() : ICmd()
{
    m_hasResponse = false;
    m_retryCount  = 0;
    Body("OK");
    HasResponse(false);
    this->m_jsontype = false;
}

TcpCmd::~TcpCmd()
{
}

void TcpCmd::SendRequestToServer()
{
    ET("TcpCmd::SendRequestToServer");
    TcpMessage* obj = gTcpManager->CreateNewResquest();

    connect(obj, SIGNAL(S_SendMessageFinished(TcpMessagePtr)), this, SLOT(R_Finished(TcpMessagePtr)));
    obj->Body(Body());
    obj->Uri(Uri());
    obj->HasResponse(HasResponse());
    obj->IsJsontype(IsJsontype());
    for (auto begin = m_headerList.begin(); begin != m_headerList.end(); begin++)
    {
        obj->AddHeader(begin.key(), begin.value());
    }
    gTcpManager->SendTcpMessage(obj);
}

void TcpCmd::MakeCmd()
{
    Body("OK");
    Uri("");
    HasResponse(false);
}

void TcpCmd::PreProcessMsg(TcpMessagePtr msg)
{
    Q_EMIT S_Finished(msg);
}

void TcpCmd::R_Finished(TcpMessagePtr msg)
{
    PreProcessMsg(msg);
}

void TcpCmd::AddHeader(QString key, QString val)
{
    m_headerList.insert(key, val);
}

QString TcpCmd::GetHeaderByKey(QString key)
{
    if (m_headerList.find(key) != m_headerList.end())
    {
        return m_headerList[key];
    }
    return "";
}
