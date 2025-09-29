#ifndef _IAGENT_H_
#define _IAGENT_H_

#include <QProcess>

class IAgent : public QObject
{
    Q_OBJECT

public:
    IAgent();
    ~IAgent();
    static IAgent *Instance(QString type, QStringList args = QStringList(), QString cmd = "");
	virtual void   StartProcess();
    virtual void   ClearProcess();
    virtual void   Execute(QString);
    virtual void   DoQuit();
    QString        GetType() const
    {
        return m_type;
    }

signals:
    void DoExit(int code, bool isCrash);
    void DoReceive(QString sender, QString receiver, QString body);

protected:
    void      Received(QString &data);
    QProcess *m_process;

    QString   m_type;
};

#endif // _IAGENT_H_
