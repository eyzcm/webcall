#ifndef OutdataConsoleAgent_H
#define OutdataConsoleAgent_H

#include "IAgent.h"

class OutdataConsoleAgent : public IAgent
{
    Q_OBJECT

private:
    OutdataConsoleAgent();
public:
	~OutdataConsoleAgent();
	static OutdataConsoleAgent* Start(QStringList args= QStringList());
	void   StartProcess();

private slots:
	void Finished( int exitCode, QProcess::ExitStatus status );
	void Output();
	void StandardError();
};

#endif // OutdataConsoleAgent_H
