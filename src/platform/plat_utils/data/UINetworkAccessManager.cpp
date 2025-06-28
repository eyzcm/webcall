#include "UINetworkAccessManager.h"
#include <QNetworkRequest>

QNetworkReply *UINetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData /*= 0*/)
{
    QNetworkRequest *req = new QNetworkRequest(request);
    req->setRawHeader("token", Token().toUtf8());
    return QNetworkAccessManager::createRequest(op, *req, outgoingData);
}
