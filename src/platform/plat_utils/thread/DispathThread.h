#ifndef DISPATHTHREAD_H
#define DISPATHTHREAD_H
#include <QMap>
#include <QMutex>
#include <QString>
#include <QThread>
#include <QTimer>
#ifdef _MSC_VER
#include <windows.h>
#endif
class DispathThread : public QThread
{
    Q_OBJECT
public:
    DispathThread();
    ~DispathThread();

signals:

    void ReceiveCmdData(QString);
    void InitTimer();

protected:
    virtual void run();
};

#endif // DISPATHTHREAD_H
