#ifndef PLATPROTOCOLSERVICE_H
#define PLATPROTOCOLSERVICE_H

#include "plat_interface_global.h"

#include <qobject.h>
#include "utils/UISingleton.h"
#include "protocol/plat_tcpmanagerinterface.h"
#include "protocol/logintcpagentinterface.h"
#include "protocol/platloginagentinterface.h"
#include "protocol/plat_monitortcpinterface.h"
#include "protocol/Plat_HttpmanagerInterface.h"
#include "protocol_biz/iplatprotocolinterface.h"
#include "serviceinterface/icommonmodelinterface.h"
#include "serviceinterface/icommonsignalinterface.h"


class PLAT_INTERFACE_EXPORT PlatProtocolService :public QObject
{

    Q_OBJECT

public:
    ~PlatProtocolService(){};
    static PlatProtocolService *GetInstance();

private:
    PlatProtocolService();
    class Garbo
    {
    public:
        ~Garbo();
    };
    static PlatProtocolService *m_instance;
public:

    void AddService( QString servername ,   void  * service );
    /*Plat_TcpManagerInterface * GetSessionService();
    PlatLoginAgentInterface  * GetLoginAgentService();
    LoginTcpAgentInterface  *  GetLoginTcpAgentService();
    Plat_MonitorTcpInterface * GetMonitorTcpService();
    Plat_HttpManagerInterface * GetHttpService();
    */
    Plat_TcpManagerInterface * GetSessionService(int index = 0);
    PlatLoginAgentInterface  * GetLoginAgentService(int index = 0);
    LoginTcpAgentInterface  *  GetLoginTcpAgentService(int index = 0);
    Plat_MonitorTcpInterface * GetMonitorTcpService(int index= 0);
    Plat_HttpManagerInterface * GetHttpService(int index=0 );

    IPlatProtocolInterface * GetPlatProtolService();
    ICommonModelInterface  * GetCommonModelService();
    ICommonSignalInterface  * GetCommonSignalService();


private:
        Plat_TcpManagerInterface * m_tcpService;
        PlatLoginAgentInterface  * m_loginAgentService;
        LoginTcpAgentInterface   * m_loginTcpService;
        Plat_MonitorTcpInterface  * m_monitorTcpService;
        Plat_HttpManagerInterface  * m_httpService;
        IPlatProtocolInterface     * m_platProtocolService;
        ICommonModelInterface      * m_commonModelInterface;
        ICommonSignalInterface     * m_commonSignalService;


        QList<Plat_TcpManagerInterface *> m_tcpServicelist;
        QList<PlatLoginAgentInterface  * >m_loginAgentServicelist;
        QList<LoginTcpAgentInterface   * >m_loginTcpServicelist;
        QList<Plat_MonitorTcpInterface  *> m_monitorTcpServicelist;
        QList<Plat_HttpManagerInterface  *> m_httpServicelist;


};



#define gTcpManager PlatProtocolService::GetInstance()->GetSessionService()
#define gVisitorMonitorTcpManager PlatProtocolService::GetInstance()->GetMonitorTcpService()

#define gIProtocolLoginAgent PlatProtocolService::GetInstance()->GetLoginAgentService()
#define gHttpManager PlatProtocolService::GetInstance()->GetHttpService()

#define gPlatProtocolService PlatProtocolService::GetInstance()->GetPlatProtolService()

#define gCommonModelService PlatProtocolService::GetInstance()->GetCommonModelService()
#define gCommonSignalService PlatProtocolService::GetInstance()->GetCommonSignalService()






#endif // PLATPROTOCOLSERVICE_H
