#ifndef IHTTPIMP_H
#define IHTTPIMP_H
#include <qobject.h>
#include "plat_interface_global.h"

class  PLAT_INTERFACE_EXPORT IHttpImp : public QObject
{
    Q_OBJECT
public:
    IHttpImp(){};
    virtual void    SetStartPos(qint64 startPos) = 0;
    virtual void    SetHeader(QString key, QString value)= 0;
    virtual void    SetExtData(QString key, QString value)= 0;
    virtual void    AsyncGetHttpData(QString strUrl)= 0;
    virtual void    AsyncPostHttpData(QString strUrl, QByteArray strData)= 0;
    virtual void    AsyncPostHttpsData(QString strUrl, QByteArray strData)= 0;
    virtual QString Requrl() const{return "";};
    virtual void    Requrl(const QString url)= 0;
    virtual void    SetSavePath(const QString path)= 0;

signals:
    void S_SourceFinished(QString code, QString reason, QByteArray body);
    void S_Finished(QString code, QString reason, QString body);
    void S_DisposeRequest();
    void S_UploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void S_DownLoadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void S_ReqFinished();

};

#endif // IHTTPIMP_H
