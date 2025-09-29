#ifndef DISPATHTHREAD_H
#define DISPATHTHREAD_H
#include <QThread>
#include <QString>

class DispathThread : public QThread
{
	Q_OBJECT
public:
	DispathThread();
	~DispathThread();
signals:
	void ReceiveCmdData(QString);
protected:
	virtual void run();
};

#endif // DISPATHTHREAD_H
