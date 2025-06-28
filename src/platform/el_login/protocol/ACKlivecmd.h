#ifndef ACKLIVECMD_H
#define ACKLIVECMD_H
#include <QObject>

#include "protocol/tcpcmd.h"

class ACKLiveCmd : public TcpCmd
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
