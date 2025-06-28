#ifndef HTTPIMP_H
#define HTTPIMP_H

#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QSslConfiguration>
#include "protocol/ihttpImp.h"

class  HttpImp : public IHttpImp
{
    Q_OBJECT

public:
    HttpImp();
    ~HttpImp();

public:
    void    SetStartPos(qint64 startPos);
    void    SetHeader(QString key, QString value);
    void    SetExtData(QString key, QString value);
    void    AsyncGetHttpData(QString strUrl);
    void    AsyncPostHttpData(QString strUrl, QByteArray strData);
    void    AsyncPostHttpsData(QString strUrl, QByteArray strData);
    QString Requrl() const;
    void    Requrl(const QString url);
    void    SetSavePath(const QString path);

public:
    void SetNetWorkManager(QNetworkAccessManager* manager);


private slots:
    void R_AuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);
    void R_readyRead();
    void R_finished();
    void R_error(QNetworkReply::NetworkError error);
    void R_uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void R_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    void InnerConnect(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_pNetWorkManager;
    QNetworkReply*         m_reply;
    QString                m_cookie;
    bool                   m_isGet;
    QMap<QString, QString> m_headerList;
    QMap<QString, QString> m_ExtList;

    QString m_requrl;
    QString m_filepath;
    qint64  m_startPos;
};

#endif // HTTPIMP_H
