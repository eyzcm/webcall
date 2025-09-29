#ifndef GFRAMEWORK_H
#define GFRAMEWORK_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QDomElement>

#include "DispathThread.h"

#define STDPRINTF(x) printf(x); printf("\n"); fflush(stdout);

class GFramework : public QObject
{
	Q_OBJECT

public:
	~GFramework();
	static GFramework* GetInstance();
private:
	GFramework();
	static GFramework* m_instance;

public:
	void Init();


	void SendCmd(QString sender, QString receiver, QString body);
	private slots:
		void R_OnReceiveCmdTask(QString cmd);

		void SubKeepAlive();
private:
	void DoReceive(QString sender, QString data);
	QString GetProtocolType( const QString& body );

	void Do_KeepAlive( const QString& body );

	void CreateTimer();


	QMap<QString, QString> Convert2Map( const QDomElement& ele );

private:
	DispathThread* m_dispThread;	

	QTimer* m_subtimer;

	bool  m_InitCef;
};
#define gFramework GFramework::GetInstance()

#endif // GFRAMEWORK_H
