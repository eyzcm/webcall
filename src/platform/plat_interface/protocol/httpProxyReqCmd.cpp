#include "httpProxyReqCmd.h"

httpProxyReqCmd::httpProxyReqCmd() : TcpCmd()
{
    m_cmd = NULL;
}

httpProxyReqCmd::~httpProxyReqCmd()
{
}

void httpProxyReqCmd::MakeCmd()
{
    QString TempUri;
    TempUri = QString("/im/web/request");

    Uri(TempUri);
}

void httpProxyReqCmd::SetHttpCmd(HttpCmd* cmd)
{
    m_cmd = cmd;
}

HttpCmd* httpProxyReqCmd::GetHttpCmd()
{
    return m_cmd;
}
