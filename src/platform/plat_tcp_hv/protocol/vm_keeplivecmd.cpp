#include "VM_KeepLiveCmd.h"

VM_KeepLiveCmd::VM_KeepLiveCmd() : VisitorMonitorTcpCmd()
{
}

VM_KeepLiveCmd::~VM_KeepLiveCmd()
{
}

void VM_KeepLiveCmd::MakeCmd()
{
    Uri("/keepalive/request");
}
