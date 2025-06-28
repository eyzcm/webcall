#ifndef HttpService_H
#define HttpService_H

#include "QMutex"

#include "httpimp.h"
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QObject>
#include <QThread>
#include <protocol/Plat_HttpManagerInterface.h>
#include <protocol/httpcmd.h>
#include "HttpWork.h"

class HttpService : public Plat_HttpManagerInterface
{
    Q_OBJECT

public:
    HttpService();
    ~HttpService();
    static HttpService* GetInstance();
    virtual void                Init();
    virtual HttpImp*            CreateHttpRequest();
    virtual void                SetPorxy(const QNetworkProxy& proxy);
    virtual void                Get(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl);
    virtual void                Post(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl, QByteArray postData);
    virtual void                HtppsPost(IHttpImp* obj, QString strUrl, QByteArray postData);


signals:
    void S_HttpReqLog(QString urlstr, QString body);


private:
    static HttpService*    m_instance;
	QThread m_workerThread;

	HttpWork * m_httpWork;
};

#define gHttpService HttpService::GetInstance()

#endif // HttpService_H
