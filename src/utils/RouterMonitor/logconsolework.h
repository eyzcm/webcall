#ifndef LOGCONSOLEWORK_H
#define LOGCONSOLEWORK_H

#include <QObject>
#include <QProcess>
class IAgent;

class LogConsoleWork : public QObject
{
    Q_OBJECT
public:
    explicit LogConsoleWork(QObject *parent = nullptr);

	void    SetAgent(IAgent *agent);

signals:

private slots:

    void R_LogInfo(QString body);


private:
	IAgent *m_agent;
};

#endif // LOGCONSOLEWORK_H
