#include "HttpWork.h"
#include "QMutex"
#include "protocol/tcpcmd.h"
#include <protocol/tcpmessage.h>
#include <protocol/HttpImp.h>
#ifdef _MSC_VER
#include <windows.h>
#include <Wininet.h>
#endif

HttpWork::HttpWork()
{
    m_manager = NULL;
	connect(this, &HttpWork::S_Init, this, &HttpWork::R_Init);
	connect(this, &HttpWork::S_Get, this, &HttpWork::R_Get);
	connect(this, &HttpWork::S_Post, this, &HttpWork::R_Post);
	connect(this, &HttpWork::S_HtppsPost, this, &HttpWork::R_HtppsPost);
}

HttpWork::~HttpWork()
{
	if (m_manager) m_manager->deleteLater();

}
void HttpWork::Init()
{
	Q_EMIT S_Init();
}
void HttpWork::R_Init()
{
	m_manager = new HttpManager();
	m_manager->Init();
	connect(m_manager, &HttpManager::S_HttpReqLog, this, &HttpWork::S_HttpReqLog);
	
}

void HttpWork::SetPorxy(const QNetworkProxy& proxy)
{
	if (m_manager) return m_manager->SetPorxy(proxy);
}

HttpImp* HttpWork::CreateHttpRequest()
{
	if (m_manager) return m_manager->CreateHttpRequest();
	return nullptr;
}

void HttpWork::Get(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl)
{
	Q_EMIT S_Get(cmdobj, objparam, strUrl);
}

void HttpWork::Post(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl, QByteArray postData)
{
	Q_EMIT S_Post(cmdobj, objparam,strUrl, postData);
}

void HttpWork::HtppsPost(IHttpImp* objparam, QString strUrl, QByteArray postData)
{
	Q_EMIT S_HtppsPost(objparam, strUrl, postData);
}


void HttpWork::R_Get(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl)
{
	if (m_manager) m_manager->Get(cmdobj, objparam, strUrl);
}

void HttpWork::R_Post(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl, QByteArray postData)
{
	if (m_manager) m_manager->Post(cmdobj, objparam, strUrl, postData);
}

void HttpWork::R_HtppsPost(IHttpImp* objparam, QString strUrl, QByteArray postData)
{
	if (m_manager) m_manager->HtppsPost(objparam, strUrl, postData);
}
