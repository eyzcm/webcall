#ifndef THREADHV_H
#define THREADHV_H

#include "ClientTcpImp.h"

#include <QObject>
#include <QThread>

class ThreadHv : public QThread
{
    Q_OBJECT
public:
    ThreadHv( ClientTcpImp * imp);

    void run();

private:
    ClientTcpImp * m_imp;


};

#endif // THREADHV_H
