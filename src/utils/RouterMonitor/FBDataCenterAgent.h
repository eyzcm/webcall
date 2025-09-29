#ifndef FBDATACENTERAGENT_H
#define FBDATACENTERAGENT_H

#include "IAgent.h"

class FBDataCenterAgent : public IAgent
{
    Q_OBJECT

private:
    FBDataCenterAgent();

public:
    ~FBDataCenterAgent();
    static FBDataCenterAgent *Start(QStringList args = QStringList(), QString cmd = "");

private slots:
    void Finished(int exitCode, QProcess::ExitStatus status);
    void Output();
    void StandardError();
};

#endif // FBDATACENTERAGENT_H
