#include "httpimp.h"

#include "utils/DefineHelper.h"

#include <QFile>
#include <QSslCertificate>
#include <QSslConfiguration>
#include "service/util_service.h"
#include "protocol/platprotocolservice.h"



HttpImp::HttpImp() : IHttpImp()
{
    m_pNetWorkManager = NULL;
    m_isGet           = false;
}

HttpImp::~HttpImp()
{
    m_isGet = false;
}

void HttpImp::SetHeader(QString key, QString value)
{
    m_headerList.insert(key, value);
}

void HttpImp::SetExtData(QString key, QString value)
{
    m_ExtList.insert(key, value);
}

void HttpImp::AsyncGetHttpData(QString strUrl)
{
    m_isGet         = true;
    QString tempUrl = strUrl;
    if (strUrl.trimmed().startsWith("http://", Qt::CaseInsensitive) || strUrl.trimmed().startsWith("https://", Qt::CaseInsensitive))
    {
        // do noting.
    }
    else
    {
        tempUrl = QString("http://%1").arg(strUrl);
    }

    GPROTOCOLLOG(tempUrl, "HttpIMPGet");
    //创建一个请求
    QSslConfiguration config;
    config.setProtocol(QSsl::TlsV1_0);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);

    QNetworkRequest request;
    QUrl            url = QUrl::fromEncoded(tempUrl.toUtf8());
    request.setUrl(url);
    for (auto begin = m_headerList.begin(); begin != m_headerList.end(); begin++)
    {
        request.setRawHeader(begin.key().toUtf8(), begin.value().toUtf8());
    }
    request.setRawHeader("Accept-Encoding", "none");

    request.setSslConfiguration(config);
    //发送GET请求
    m_reply = m_pNetWorkManager->get(request);
    InnerConnect(m_reply);
    gIProtocolLoginAgent->DoHttpReqLog(strUrl, "");

}

void HttpImp::AsyncPostHttpData(QString strUrl, QByteArray strData)
{
    QString tempUrl = strUrl;
    QString valInfo = QString("start begin post url:%1\r\nbody:%2").arg(strUrl).arg(QString(strData));

    GPROTOCOLLOG(valInfo, "HttpIMPPost");

    m_isGet = false;
    //创建一个请求
    QNetworkRequest request;
    QUrl            url = QUrl::fromEncoded(tempUrl.toUtf8());
    request.setUrl(url);

    QSslConfiguration config;
    config.setProtocol(QSsl::TlsV1_0);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);

    request.setSslConfiguration(config);

    for (auto begin = m_headerList.begin(); begin != m_headerList.end(); begin++)
    {
        request.setRawHeader(begin.key().toUtf8(), begin.value().toUtf8());
    }
    //发送post请求
    m_reply = m_pNetWorkManager->post(request, strData);
    InnerConnect(m_reply);
    gIProtocolLoginAgent->DoHttpReqLog(strUrl, QString::fromUtf8(strData));

}

void HttpImp::AsyncPostHttpsData(QString strUrl, QByteArray strData)
{

    QSslConfiguration config;
    // QList<QSslCertificate> certs = QSslCertificate::fromPath(certificateFilePath);
    // config.setCaCertificates(certs);
    config.setProtocol(QSsl::TlsV1_0);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);

    QString tempUrl = strUrl;
    QString valInfo = QString("start begin post url:%1\r\nbody:%2").arg(strUrl).arg(QString(strData));
    GPROTOCOLLOG(valInfo, "HttpIMPPost");
    m_isGet = false;
    //创建一个请求
    QNetworkRequest request;
    QUrl            url = QUrl::fromEncoded(tempUrl.toUtf8());
    request.setUrl(url);
    // request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" ); //强制文本，修复服务器不能读取的问题，如果是数据流另做
    for (auto begin = m_headerList.begin(); begin != m_headerList.end(); begin++)
    {
        request.setRawHeader(begin.key().toUtf8(), begin.value().toUtf8());
    }

    request.setSslConfiguration(config);
    //发送post请求
    m_reply = m_pNetWorkManager->post(request, strData);
    InnerConnect(m_reply);
    gIProtocolLoginAgent->DoHttpReqLog(strUrl, QString::fromUtf8(strData));
}

void HttpImp::InnerConnect(QNetworkReply* reply)
{
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(R_readyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(R_downloadProgress(qint64, qint64)));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(R_error(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(R_finished()));
    connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(R_uploadProgress(qint64, qint64)));
}

void HttpImp::SetNetWorkManager(QNetworkAccessManager* manager)
{
    m_pNetWorkManager = manager;
    connect(m_pNetWorkManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(R_AuthenticationRequired(QNetworkReply*, QAuthenticator*)));
}

void HttpImp::R_AuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
    // app_key:ik2AbaPxksM=
    // app_secret：kn5CWK0+PA7LOs/ZwpLsyA==
    authenticator->setUser(m_ExtList["username"]);
    authenticator->setPassword(m_ExtList["password"]);
}

void HttpImp::R_readyRead()
{
}

void HttpImp::R_finished()
{
    Q_EMIT S_ReqFinished();
    if (m_reply == NULL)
    {
        Q_EMIT S_Finished("600", "unkown error", "");

        gIProtocolLoginAgent->DoHttpResultFinishedLog("600", "unkown error", "");
        Q_EMIT S_DisposeRequest();
        return;
    }

    int dCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // GLOG(QString( "Http return code:%1" ).arg( dCode ));

    int val = m_reply->header(QNetworkRequest::ContentLengthHeader).toInt();

    int err = m_reply->error();
    if (m_reply->error() == QNetworkReply::NoError || ((m_reply->error() == QNetworkReply::RemoteHostClosedError)))
    {
        if ((dCode == 200) || (dCode == 206))
        {

            QByteArray arrayVal;
            if (m_filepath.isEmpty() == false)
            {
                QFile f(m_filepath);
                f.open(QFile::ReadWrite);
                if (f.isOpen())
                {
                    f.seek(m_startPos);
                }
                for (;;)
                {
                    QByteArray data = m_reply->read(1024);

                    if (f.isOpen())
                    {

                        f.write(data.constData(), data.length());
                    }
                    if (data.length() < 1024)
                        break;
                }
                if (f.isOpen())
                    f.close();
            }
            else
                arrayVal = m_reply->readAll();

            QString httpBody = QString::fromUtf8(arrayVal);
            Q_EMIT S_SourceFinished("200", "ok", arrayVal);
            Q_EMIT S_Finished("200", "ok", httpBody);

            gIProtocolLoginAgent->DoHttpResultFinishedLog("200", "ok", httpBody);


            m_reply->deleteLater();
            m_reply = NULL;
        }
        else if (dCode == 302 || dCode == 301)
        {
            if (m_isGet)
            {
                QString strUrl = m_reply->rawHeader("Location");
                AsyncGetHttpData(strUrl);
                return;
            }
        }
        else
        {
            Q_EMIT S_SourceFinished(QString("%1").arg(dCode), m_reply->errorString(), "");
            Q_EMIT S_Finished(QString("%1").arg(dCode), m_reply->errorString(), "");

            gIProtocolLoginAgent->DoHttpResultFinishedLog(QString("%1").arg(dCode), m_reply->errorString(), "");
        }
    }
    else if (err == QNetworkReply::AuthenticationRequiredError)
    {
    }
    else
    {

        if (dCode == 302 || dCode == 301)
        {
            if (m_isGet)
            {
                QString strUrl = m_reply->rawHeader("Location");
                AsyncGetHttpData(strUrl);
                return;
            }
            else
            {

                Q_EMIT S_SourceFinished(QString("%1").arg(dCode), m_reply->errorString(), "");
                Q_EMIT S_Finished(QString("%1").arg(dCode), m_reply->errorString(), "");

                gIProtocolLoginAgent->DoHttpResultFinishedLog(QString("%1").arg(dCode), m_reply->errorString(), "");
            }
        }
        else
        {

            Q_EMIT S_SourceFinished(QString("%1").arg(dCode), m_reply->errorString(), "");
            Q_EMIT S_Finished(QString("%1").arg(dCode), m_reply->errorString(), "");

            gIProtocolLoginAgent->DoHttpResultFinishedLog(QString("%1").arg(dCode), m_reply->errorString(), "");
        }
    }
    Q_EMIT S_DisposeRequest();
}

void HttpImp::R_error(QNetworkReply::NetworkError error)
{
    // GLOG(QString("NetworkError error code: %d").arg(error));
}

void HttpImp::R_uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
}

void HttpImp::R_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_EMIT S_DownLoadProgress(bytesReceived, bytesTotal);
}

QString HttpImp::Requrl() const
{
    return m_requrl;
}

void HttpImp::Requrl(const QString url)
{
    m_requrl = url;
}

void HttpImp::SetSavePath(const QString path)
{
    m_filepath = path;
}

void HttpImp::SetStartPos(qint64 startPos)
{
    m_startPos = startPos;
}
