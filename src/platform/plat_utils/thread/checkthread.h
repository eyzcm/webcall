#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H
#include <QMap>
#include <QMutex>
#include <QThread>
#include <QTimer>
#ifdef _MSC_VER
#include <Windows.h>
#endif
class CheckThread : public QThread
{
    Q_OBJECT

public:
    CheckThread(QObject *parent);
    ~CheckThread();
    void SetTimer(QString key, unsigned long time);

signals:
    void S_RunCheck();

public slots:
    void R_OnTimer();

protected:
    QTimer                      *m_timer;
    bool                      CheckRun();

    virtual void                 run();
    QMap<QString, unsigned long> maptimes;
    QMutex                       m_mutex;
};

#endif // CHECKTHREAD_H
