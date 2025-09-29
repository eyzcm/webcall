#ifndef OutdataConsoleWork_H
#define OutdataConsoleWork_H

#include <QObject>
#include <QProcess>
class IAgent;

class OutdataConsoleWork : public QObject
{
    Q_OBJECT
public:
    explicit OutdataConsoleWork(QObject *parent = nullptr);

	void    SetAgent(IAgent *agent);

signals:

private slots:

    void R_LogInfo(QString body);


private:
	IAgent *m_agent;
};

#endif // OutdataConsoleWork_H
