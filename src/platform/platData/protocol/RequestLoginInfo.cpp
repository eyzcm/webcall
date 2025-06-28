#include "RequestLoginInfo.h"
#include <QDomDocument>

RequestLoginInfo::RequestLoginInfo(void)
{
}

RequestLoginInfo::~RequestLoginInfo(void)
{
}

QString RequestLoginInfo::MakeXml()
{
    QDomDocument doc;
    QDomElement  root = doc.createElement("bizlogininfo");
    doc.appendChild(root);

    QDomElement tag;
    QDomText    t;
#define AddElement(x, y)        \
    tag = doc.createElement(x); \
    root.appendChild(tag);      \
    t = doc.createTextNode(y);  \
    tag.appendChild(t);

    AddElement("username", UserName());
    AddElement("password", Password());
    AddElement("servers", ConnectService());
    AddElement("port", Port());
    QString body = doc.toString();
    return body;
}
