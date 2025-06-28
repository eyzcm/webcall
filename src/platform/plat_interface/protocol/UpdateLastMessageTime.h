#ifndef UpdateLastMessageTime_H
#define UpdateLastMessageTime_H


#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT UpdateLastMessageTime : public TcpCmd
{
    Q_OBJECT

public:
    UpdateLastMessageTime(QString chatId);
    ~UpdateLastMessageTime();

    virtual void MakeCmd();

private:
    QString m_chatId;
};

#endif // UpdateLastMessageTime_H
