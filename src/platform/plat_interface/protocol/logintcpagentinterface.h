#ifndef LOGINTCPAGENTINTERFACE_H
#define LOGINTCPAGENTINTERFACE_H

#include <qobject.h>
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT LoginTcpAgentInterface  : public QObject
{
public:
    LoginTcpAgentInterface();


    virtual void Init(QString loginurl, QString port) = 0;
    virtual void CreateTcpConnect(QString loginurl, QString port) = 0;
    virtual void Clear() = 0;
    virtual void TcpReconnect() = 0;


signals:
    void S_connected();
    void S_NetWorkError(int errorCode,QString msg);
    void S_MessageParsingFailed();

    void S_LoginFinished(QString authType, QString errorCode,QString body);
    void S_ForceLogout();
    void S_ForceReLogin(int force);



};

#endif // LOGINTCPAGENTINTERFACE_H
