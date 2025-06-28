#include "VM_AuthCmd.h"
#include "protocol_biz/VisitorMonitorLoginTcpAgent.h"
#include "protocol/serviceaddressmanager.h"
#include "visitormonitortcpmanager.h"

VM_AuthCmd::VM_AuthCmd() : VisitorMonitorTcpCmd()
{
}

VM_AuthCmd::~VM_AuthCmd()
{
}

void VM_AuthCmd::MakeCmd()
{
    Uri("/auth");
}

void VM_AuthCmd::PreProcessMsg(TcpMessagePtr msg)
{
    if (msg == nullptr)
    {
        Q_EMIT S_Finished(msg);
        return;
    }
    if (msg->GetHeaderByName("Error") == "301")
    {
        QString serverList = msg->GetHeaderByName("Servers");
        ServiceAddressManager::GetInstance()->ClearAddressInfoList(1);
        ServiceAddressManager::GetInstance()->SetServiceList(serverList, 1);
        addressinfo* info = ServiceAddressManager::GetInstance()->GetMonitorAddressInfo();
        if (info)
        {
            gVisitorMonitorTcpHvManager->CreateTcpConnect(info->m_host, info->m_port);
        }

        if (msg)
        {
            msg->deleteLater();
            msg = nullptr;
        }
    }
    else
    {
        gVisitorMonitorLoginTcpAgent->DoExecVMAuthLoginCmdResult(msg);
        // Q_EMIT S_Finished(msg);
    }
}
