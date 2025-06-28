#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include "QMutex"

#include "httpimp.h"
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QObject>
#include <protocol/Plat_HttpManagerInterface.h>
#include <protocol/httpcmd.h>

class HttpManager : public QObject
{
    Q_OBJECT

public:
    HttpManager();
    ~HttpManager();
    static HttpManager* GetInstance();
    virtual void                Init();
    virtual HttpImp*            CreateHttpRequest();
    virtual void                SetPorxy(const QNetworkProxy& proxy);
    virtual void                Get(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl);
    virtual void                Post(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl, QByteArray postData);
    virtual void                HtppsPost(IHttpImp* obj, QString strUrl, QByteArray postData);

private:
    QNetworkAccessManager* InnerManager();

signals:
    void S_HttpReqLog(QString urlstr, QString body);

private slots:
    void R_DisposeRequest();
    void R_Reqfinished();

private:
    static HttpManager*    m_instance;
    QList<HttpImp*>        m_requestList;
    QNetworkAccessManager* m_manager;
    QString                m_cookie;
    QMutex                 m_mutex;
    QMutex                 m_reqmutex;
    int                    m_maxPool;
    QList<HttpCmd*>        m_httpCmdList;
    int                    m_currCount;
};



#endif // HTTPMANAGER_H
