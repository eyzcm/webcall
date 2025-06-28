#include "RequestLogoutInfo.h"
#include <QDomDocument>

RequestLogoutInfo::RequestLogoutInfo(void)
{
}

RequestLogoutInfo::~RequestLogoutInfo(void)
{
}

QString RequestLogoutInfo::MakeXml()
{
    QDomDocument doc;
    QDomElement  root = doc.createElement("bizlogoutinfo");
    doc.appendChild(root);
    QString body = doc.toString();
    return body;
}
