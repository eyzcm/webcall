#ifndef OFFLINEMESSAGE_H
#define OFFLINEMESSAGE_H


#include "protocol/tcpcmd.h"

class  OfflineMessageCmd : public TcpCmd
{
    Q_OBJECT

public:
    OfflineMessageCmd();
    ~OfflineMessageCmd();

    virtual void MakeCmd();

private:
};

#endif // OFFLINEMESSAGE_H
