#include "RequestKeepLive.h"
#include <QDomDocument>
#include "ConstStringHelper.h"


RequestKeepLive::RequestKeepLive(void)	
{
}


RequestKeepLive::~RequestKeepLive(void)
{
}

QString RequestKeepLive::MakeXml()
{
	QDomDocument doc;
	QDomElement root = doc.createElement(ConstStringHelper::BizKeepAlive);
	doc.appendChild(root);
	QString body = doc.toString();
	return body;
}
