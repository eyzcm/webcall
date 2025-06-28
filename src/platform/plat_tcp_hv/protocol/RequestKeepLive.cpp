#include "RequestKeepLive.h"
#include "utils/ConstStringHelper.h"

#include <QtXml/QDomDocument>


RequestKeepLive::RequestKeepLive(void)
{
}

RequestKeepLive::~RequestKeepLive(void)
{
}

QString RequestKeepLive::MakeXml()
{
    QDomDocument doc;
    QDomElement  root = doc.createElement(ConstStringHelper::BizKeepAlive);
    doc.appendChild(root);
    QString body = doc.toString();
    return body;
}
