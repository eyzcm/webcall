#ifndef VM_KEEPLIVECMD_H
#define VM_KEEPLIVECMD_H


#include "protocol/visitormonitortcpcmd.h"

class  VM_KeepLiveCmd : public VisitorMonitorTcpCmd
{
    Q_OBJECT

public:
    VM_KeepLiveCmd();
    ~VM_KeepLiveCmd();

    virtual void MakeCmd();

private:
};

#endif // VM_KEEPLIVECMD_H
