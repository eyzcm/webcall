#ifndef IRESPONSE_H
#define IRESPONSE_H

#include <stdio.h>
#include <wchar.h>
#include <QObject>

#define ResponseObjectName(x)\
	public:\
	virtual QString ObjectName() const { return x; }

#define STDPRINTF(x) printf(x); printf("\n"); fflush(stdout);
#define STDERR(x) fprintf(stderr, x); fprintf(stderr,"\n"); fflush(stderr);



class IResponse:public QObject
{
	Q_OBJECT
public:
	IResponse(void);
	~IResponse(void);
public:
	void SetReq(QString val) { m_req = val; }
	QString GetReq() { return m_req; }
	
	QString State() const { return m_state; }
	void State(QString val) { m_state = val; }

	QString Receiver() const { return m_receiver; }
	void Receiver(QString val) { m_receiver = val; }
public:
	virtual void Execute(QString );    
	virtual QString ObjectName() const = 0 ;
	virtual QString MakeXml() = 0;		 
private:
	QString m_req;	
	QString m_state;
	QString m_receiver;
};
#endif //IRESPONSE_H