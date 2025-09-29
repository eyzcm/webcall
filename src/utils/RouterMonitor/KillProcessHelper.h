#ifndef KILLPROCESSHELPER_H
#define KILLPROCESSHELPER_H
#include <QObject>
#include <QProcess>

class KillProcessHelper : public QObject
{
    Q_OBJECT

public:
    virtual ~KillProcessHelper();

private:
    KillProcessHelper();

public:
    static void DoExit(Q_PID dPid, int ncode);

    static void AdjustPrivilege();

private:
};

#endif // KILLPROCESSHELPER_H
