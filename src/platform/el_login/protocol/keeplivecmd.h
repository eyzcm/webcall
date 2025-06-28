#ifndef KEEPLIVECMD_H
#define KEEPLIVECMD_H


#include "protocol/tcpcmd.h"

class KeepLiveCmd : public TcpCmd
{
    Q_OBJECT

public:
    KeepLiveCmd();
    ~KeepLiveCmd();

    virtual void MakeCmd();

private:
};

#endif // KEEPLIVECMD_H
