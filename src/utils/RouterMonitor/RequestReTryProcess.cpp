#include "RequestReTryProcess.h"
#include "ConstStringHelper.h"
#include <QDomDocument>

RequestReTryProcess::RequestReTryProcess(void) : m_type("")
{
}

RequestReTryProcess::~RequestReTryProcess(void)
{
}

QString RequestReTryProcess::MakeXml()
{
    QDomDocument doc;
    QDomElement  root = doc.createElement(ConstStringHelper::BizReTryProcess);
    root.setAttribute(ConstStringHelper::req, GetReq());
    doc.appendChild(root);
    QDomElement tag = doc.createElement(ConstStringHelper::Source);
    root.appendChild(tag);
    QDomText t = doc.createTextNode(m_type);
    tag.appendChild(t);
    QString body = doc.toString();
    return body;
}
