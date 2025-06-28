#ifndef KEEPLIVECMD_H
#define KEEPLIVECMD_H


#include "tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT KeepLiveCmd : public TcpCmd
{
    Q_OBJECT

public:
    KeepLiveCmd();
    ~KeepLiveCmd();

    virtual void MakeCmd();

private:
};

#endif // KEEPLIVECMD_H
