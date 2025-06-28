#ifndef PLAT_MONITORTCPINTERFACE_H
#define PLAT_MONITORTCPINTERFACE_H

#include <qobject.h>
#include <qstring.h>
#include <protocol/tcpmessage.h>


#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT Plat_MonitorTcpInterface :public QObject
{
      Q_OBJECT
public:
    Plat_MonitorTcpInterface();
     ~Plat_MonitorTcpInterface(){};

    virtual void CreateTcpConnect( QString host ,QString port) = 0;
    virtual void CreateTimer() = 0;
    virtual void CloseTimer()= 0;
    virtual void DisconnectTcp()= 0;

    virtual TcpMessagePtr CreateNewResquest(){  return nullptr;};

    virtual void SendTcpMessage(TcpMessagePtr msg) = 0;

    virtual void LoginOk() = 0;

signals:
    void S_TCP_SendTcpMessage(QByteArray msg);
    void S_connected();
    void S_NetWorkError(int code, QString reason);

    void S_MessageParsingFailed();
    void S_ReceivedMessage(TcpMessagePtr);



};

#endif // PLAT_MONITORTCPINTERFACE_H
