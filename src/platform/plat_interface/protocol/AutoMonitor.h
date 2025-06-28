#ifndef AutoMonitor_H
#define AutoMonitor_H

#include <qobject.h>
#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT AutoMonitor : public TcpCmd
{
    Q_OBJECT

public:
    AutoMonitor(QString chatId);
    ~AutoMonitor();

    virtual void MakeCmd();

private:
    QString m_chatId;
};

#endif // AutoMonitor_H
