#ifndef PLAT_HTTPMANAGERINTERFACE_H
#define PLAT_HTTPMANAGERINTERFACE_H
#include <protocol/IHttpImp.h>
#include <protocol/HttpCmd.h>

#include <QNetworkProxy.h>
#include "plat_interface_global.h"
#include <qobject.h>

class PLAT_INTERFACE_EXPORT Plat_HttpManagerInterface: public QObject
{

public:
    Plat_HttpManagerInterface(){};

    virtual void                Init(){};
    virtual     IHttpImp*            CreateHttpRequest(){return NULL;};
    virtual void                SetPorxy(const QNetworkProxy& proxy){};
    virtual void                Get(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl){return ;};
    virtual void                Post(HttpCmd* cmdobj, IHttpImp* obj, QString strUrl, QByteArray postData){return ;};
    virtual void                HtppsPost(IHttpImp* obj, QString strUrl, QByteArray postData){return ;};

signals:
    void S_HttpReqLog(QString urlstr, QString body);



};

#endif // PLAT_HTTPMANAGERINTERFACE_H
