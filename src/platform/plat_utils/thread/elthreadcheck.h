#ifndef ELTHREADCHECK_H
#define ELTHREADCHECK_H

#include <QThread>

class ElThreadCheck : public QThread
{
    Q_OBJECT

public:
    ElThreadCheck();
    ~ElThreadCheck();
    void setThreadName(QString name);
    void InitConnect();
	void SetMainObject(QObject * mainobj);

signals:
	void S_ThreadInit();

private slots:
    void R_RunCheck();

private:
    QString m_threadname;
	QObject * m_mainobj;

};

#endif // ELTHREADCHECK_H
