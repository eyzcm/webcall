#ifndef LogConsoleAgent_H
#define LogConsoleAgent_H

#include "IAgent.h"

class LogConsoleAgent : public IAgent
{
    Q_OBJECT

private:
    LogConsoleAgent();
public:
	~LogConsoleAgent();
	static LogConsoleAgent* Start(QStringList args= QStringList());
	void   StartProcess();

private slots:
	void Finished( int exitCode, QProcess::ExitStatus status );
	void Output();
	void StandardError();
};

#endif // LogConsoleAgent_H
