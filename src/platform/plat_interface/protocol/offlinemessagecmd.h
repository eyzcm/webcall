#ifndef OFFLINEMESSAGE_H
#define OFFLINEMESSAGE_H


#include "tcpcmd.h"
#include "plat_interface_global.h"

class  PLAT_INTERFACE_EXPORT OfflineMessageCmd : public TcpCmd
{
    Q_OBJECT

public:
    OfflineMessageCmd();
    ~OfflineMessageCmd();

    virtual void MakeCmd();

private:
};

#endif // OFFLINEMESSAGE_H
