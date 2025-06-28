#ifndef VM_AUTHCMD_H
#define VM_AUTHCMD_H

#include "protocol/visitormonitortcpcmd.h"


class  VM_AuthCmd : public VisitorMonitorTcpCmd
{
    Q_OBJECT

public:
    VM_AuthCmd();
    ~VM_AuthCmd();

    virtual void MakeCmd();

    virtual void PreProcessMsg(TcpMessagePtr msg);

private:
};

#endif // VM_AUTHCMD_H
