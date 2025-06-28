#ifndef HTTPPROXYREQCMD_H
#define HTTPPROXYREQCMD_H


#include "protocol/httpcmd.h"
#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT httpProxyReqCmd : public TcpCmd
{

    Q_OBJECT

public:
    httpProxyReqCmd();
    ~httpProxyReqCmd();

    virtual void MakeCmd();
    void         SetHttpCmd(HttpCmd* cmd);
    HttpCmd*     GetHttpCmd();

public:
    // to为目标客服
    Property_QString(Cookies, cookies);
    Property_QString(ReqUrl, reqUrl);

public:
private:
    HttpCmd* m_cmd;
};

#endif // httpProxyReqCmd_H
