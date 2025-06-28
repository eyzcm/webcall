#ifndef HTTPCMD_H
#define HTTPCMD_H
#include "utils/ConstStringHelper.h"
#include "utils/DefineHelper.h"
#include "utils/Plat_EnumDefine.h"

#include <QMap>
#include <QObject>
#include "plat_interface_global.h"

#include <QTimer>

class Plat_HttpManagerInterface;
class IHttpImp;

class  PLAT_INTERFACE_EXPORT HttpCmd : public QObject
{
    Q_OBJECT

public:
    HttpCmd();
    ~HttpCmd();
    void ReInit();

public:
    void ImmediateSend(int type);
    int  ImmediateSend();
    void ReReqData();
    void Get();
    void Post(const QByteArray& data);
    void HttpsPost(const QByteArray& data, const QString &filePath);
    void setHttpManager(Plat_HttpManagerInterface* hm = nullptr);

    QString GetUriWithBody();
    void    SetHeader(const QString& key, const QString& val);

    void    AddCmdData(const QString& key, const QString& val);
    QString GetCmdDataByKey(const QString& key);

    void  AddCacheByKey(const QString& key, void* data);
    void* GetCacheByKey(const QString& key);
    void  DoFinished(QString code, QString reason, QString rsp);

    QMap<QString, QString> getm_headerList()
    {
        return m_headerList;
    }
    QMap<QString, QString> getm_cmdDataList()
    {
        return m_cmdDataList;
    }

    void timeoutRetry();
    void deleteTime();

public:
    Property_QString(Uri, uri);
    Property_QString(FileName, filename);
    Property_QString(UserId, userid);
    Property_QString(CmdID, cmdID);

signals:
    void S_SourceFinished(QString code, QString reason, QByteArray body);
    void S_Finished(QString code, QString reason, QString rsp);
    void S_DownLoadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void S_AsyncGetHttpData(QString myId, QMap<QString, QString> m_h, QMap<QString, QString> m_E, QString strUrl);
    void S_AsyncPostHttpData(QString myId, QMap<QString, QString> m_h, QMap<QString, QString> m_E, QString strUrl, QByteArray strData);

public slots:
    void R_SourceFinished(QString code, QString reason, QByteArray body);
    void R_Finished(QString code, QString reason, QString rsp);
    void R_DownLoadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void R_timeoutRetry();

private:
    QMap<QString, QString> m_headerList;
    QMap<QString, QString> m_cmdDataList;
    QMap<QString, void*>   m_cmdCacheData;
    QByteArray             m_body;
    int                    m_reqType;
    int                    m_ImmediateSend;
    Plat_HttpManagerInterface*           m_HttpManager;
    QTimer*                m_time;
    QList<IHttpImp * >      m_objList;
};

#endif // HTTPCMD_H
