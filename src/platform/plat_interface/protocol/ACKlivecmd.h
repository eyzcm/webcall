#ifndef ACKLIVECMD_H
#define ACKLIVECMD_H
#include <QObject>

#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT ACKLiveCmd : public TcpCmd
{
    Q_OBJECT

public:
    ACKLiveCmd(QString chatId);
    ~ACKLiveCmd();

    virtual void MakeCmd();

private:
    QString m_chatId;
};

#endif // ACKLIVECMD_H
