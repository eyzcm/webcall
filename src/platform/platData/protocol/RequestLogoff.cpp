#include "RequestLogoff.h"
#include <QDomDocument>

RequestLogoff::RequestLogoff(void)
{
}

RequestLogoff::~RequestLogoff(void)
{
}

QString RequestLogoff::MakeXml()
{
    QDomDocument doc;
    QDomElement  root = doc.createElement("bizlogoff");
    doc.appendChild(root);
    QString body = doc.toString();
    return body;
}
