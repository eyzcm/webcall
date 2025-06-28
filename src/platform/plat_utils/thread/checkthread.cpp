#include "checkthread.h"
#include "QApplication"
#include <qdebug.h>

CheckThread::CheckThread(QObject *parent) : QThread(parent)
{
    m_timer = nullptr;
    qDebug()<<"moveToThread CheckThread"<<endl;
    moveToThread(this);
    start();
}

CheckThread::~CheckThread()
{
}

void CheckThread::run()
{
    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(R_OnTimer()));
    m_timer->setInterval(1 * 1000);
    m_timer->start();
    exec();
}

bool CheckThread::CheckRun()
{
 #ifdef _MSC_VER   
    QMutexLocker  locker(&m_mutex);
    unsigned long curcount = GetTickCount();
    foreach (auto iter, maptimes)
    {
        unsigned long tcount = iter;
        if ((curcount - tcount) > 60000)
        {
            return false;
        }
    }
#endif    
    return true;
}

void CheckThread::SetTimer(QString key, unsigned long time)
{
    QMutexLocker locker(&m_mutex);
    maptimes[key] = time;
}

void CheckThread::R_OnTimer()
{
    if (CheckRun())
    {
        Q_EMIT S_RunCheck();
    }
    else
    {
        //	Q_ASSERT(false);
    }
}
