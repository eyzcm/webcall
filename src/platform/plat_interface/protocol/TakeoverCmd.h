#ifndef TakeoverCmd_H
#define TakeoverCmd_H


#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT TakeoverCmd : public TcpCmd
{
    Q_OBJECT

public:
    TakeoverCmd(QString chatId);
    ~TakeoverCmd();

    virtual void MakeCmd();

private:
    QString m_chatId;
};

#endif // TakeoverCmd_H
