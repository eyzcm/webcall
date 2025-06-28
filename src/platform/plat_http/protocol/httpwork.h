#ifndef HttpWork_H
#define HttpWork_H

#include "QMutex"

#include "httpimp.h"
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QObject>
#include <protocol/Plat_HttpManagerInterface.h>
#include <protocol/httpcmd.h>
#include "HttpManager.h"

class HttpWork : public QObject
{
    Q_OBJECT

public:
    HttpWork();
    ~HttpWork();
    
    virtual void                Init();
    virtual HttpImp*            CreateHttpRequest();
    virtual void                SetPorxy(const QNetworkProxy& proxy);
    virtual void                Get(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl);
    virtual void                Post(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl, QByteArray postData);
    virtual void                HtppsPost(IHttpImp* obj, QString strUrl, QByteArray postData);

signals:
	void S_Init();
    void S_HttpReqLog(QString urlstr, QString body);
	void S_Get(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl);
	void S_Post(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl, QByteArray postData);
	void S_HtppsPost(IHttpImp* obj, QString strUrl, QByteArray postData);

private slots:
	void R_Init();
	void R_Get(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl);
	void R_Post(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl, QByteArray postData);
	void R_HtppsPost(IHttpImp* obj, QString strUrl, QByteArray postData);

private:
	HttpManager * m_manager;
};



#endif // HttpWork_H
