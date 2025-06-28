#ifndef VisitorMonitorTcpHvManager_H
#define VisitorMonitorTcpHvManager_H

#include <QObject>
#include <QTimer>



#include "protocol/tcpmessage.h"

#include "protocol/visitormonitortcpcmd.h"
#include <QMutex>

#include <protocol/plat_monitortcpinterface.h>

class ElVmStatusProcess;


class  VisitorMonitorTcpManager : public Plat_MonitorTcpInterface
{
    Q_OBJECT

public:
    static VisitorMonitorTcpManager* Instance();
    ~VisitorMonitorTcpManager();

private:
    VisitorMonitorTcpManager();
    static VisitorMonitorTcpManager* m_instance;

public:
    void CreateTcpConnect(QString host, QString port);
    void CreateTimer();
    void CloseTimer();
    void DisconnectTcp();

    TcpMessagePtr CreateNewResquest();

    void SendTcpMessage(TcpMessagePtr msg);

    void LoginOk();

signals:
    void s_test();
    

private slots:
    void R_connected();
    void R_NetWorkError(int code, QString reason);

    void R_OnTimer();
    void R_ReceivedMessage(TcpMessagePtr msg);

private:
    int         GetNextCallId();
    TcpMessagePtr FindTcpMessage(TcpMessagePtr msg);

private:
    QList<TcpMessagePtr> m_reqList;

    int                m_callID;
    ElVmStatusProcess* m_tcpImp;
    QTimer*            m_timer;
    QMutex             m_mutex;
};

#define gVisitorMonitorTcpHvManager VisitorMonitorTcpManager::Instance()

#endif // VisitorMonitorTcpHvManager_H
