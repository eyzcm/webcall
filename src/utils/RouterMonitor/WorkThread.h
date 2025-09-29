#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include "IAgent.h"


class WorkThread:public QThread
{
    Q_OBJECT
public:
    WorkThread();

	IAgent * m_agent;

private:
    virtual void run();



};

#endif // WORKTHREAD_H
