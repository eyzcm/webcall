#include "offlinemessagecmd.h"

OfflineMessageCmd::OfflineMessageCmd() : TcpCmd()
{
}

OfflineMessageCmd::~OfflineMessageCmd()
{
}

void OfflineMessageCmd::MakeCmd()
{
    Uri("/im/offline");
    Body("offline");
}
