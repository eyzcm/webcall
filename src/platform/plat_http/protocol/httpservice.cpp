#include "HttpService.h"
#include "QMutex"
#include "protocol/tcpcmd.h"
#include <protocol/tcpmessage.h>
#include <protocol/HttpImp.h>
#ifdef _MSC_VER
#include <windows.h>
#include <Wininet.h>
#endif

HttpService* HttpService::m_instance = nullptr;

HttpService::HttpService()
{
	m_httpWork = nullptr;
}

HttpService::~HttpService()
{
}

HttpService* HttpService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new HttpService;
    }
    return m_instance;
}

void HttpService::Init()
{
	if (m_httpWork == nullptr)
	{

		m_workerThread.start();

		// 在子线程中执行网络请求
		m_httpWork = new HttpWork();
		m_httpWork->moveToThread(&m_workerThread);
		QObject::connect(&m_workerThread, &QThread::finished, m_httpWork, &QObject::deleteLater);
		connect(m_httpWork, &HttpWork::S_HttpReqLog, this, &HttpService::S_HttpReqLog);
		m_httpWork->Init();
	}
	
}

void HttpService::SetPorxy(const QNetworkProxy& proxy)
{
	if (m_httpWork) m_httpWork->SetPorxy(proxy);
    
}

HttpImp* HttpService::CreateHttpRequest()
{
	if (m_httpWork) return m_httpWork->CreateHttpRequest();
    return nullptr;
}

void HttpService::Get(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl)
{
	if (m_httpWork) return m_httpWork->Get(cmdobj,objparam,strUrl);
    
}

void HttpService::Post(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl, QByteArray postData)
{
	if (m_httpWork) return m_httpWork->Post(cmdobj, objparam, strUrl,postData);
    
}

void HttpService::HtppsPost(IHttpImp* objparam, QString strUrl, QByteArray postData)
{
	if (m_httpWork) return m_httpWork->HtppsPost( objparam, strUrl, postData);
}

