#include "keeplivecmd.h"

KeepLiveCmd::KeepLiveCmd() : TcpCmd()
{
}

KeepLiveCmd::~KeepLiveCmd()
{
}

void KeepLiveCmd::MakeCmd()
{
    Uri("/im/keepAlive?receipt=1");
    Body("keepAlive");
}
