#ifndef PROTOCOLTHREAD_H
#define PROTOCOLTHREAD_H

#include "./thread/ElThreadCheck.h"

#include <QObject>
#include <QThread>

class  ProtocolThread : public ElThreadCheck
{
    Q_OBJECT

public:
    ProtocolThread();
    ~ProtocolThread();

private:
    virtual void run();
};

#endif // PROTOCOLTHREAD_H
