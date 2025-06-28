#include "httpmanager.h"
#include "QMutex"
#include "protocol/tcpcmd.h"
#include <protocol/tcpmessage.h>
#include <protocol/HttpImp.h>
#ifdef _MSC_VER
#include <windows.h>
#include <Wininet.h>
#else
#ifndef DWORD
#define DWORD unsigned long
#endif
#endif

HttpManager* HttpManager::m_instance = nullptr;

HttpManager::HttpManager()
{
    m_manager = NULL;
    InnerManager();
    m_maxPool   = 80;
    m_currCount = 0;
    QMutexLocker locker(&m_reqmutex);
}

HttpManager::~HttpManager()
{
}

HttpManager* HttpManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new HttpManager;
    }
    return m_instance;
}

void HttpManager::Init()
{
    QString maxPool = "80";// gUISystemConfigManager->GetSystemConfigByKey("maxhttppool", "80");

    int pool = maxPool.toInt();
    if (pool > 0)
        m_maxPool = pool;
}

void HttpManager::SetPorxy(const QNetworkProxy& proxy)
{
    if (proxy.type() != QNetworkProxy::NoProxy)
    {
        m_manager->proxy().setApplicationProxy(proxy);
    }
}

HttpImp* HttpManager::CreateHttpRequest()
{
    QMutexLocker locker(&m_mutex);

    DWORD dwSize = 0;
    static  bool isProxy = false;

        bool ret = true;
#ifdef _MSC_VER        
        InternetQueryOptionA(NULL, INTERNET_OPTION_PROXY, NULL, &dwSize);
        if( dwSize > 0 )
        {

            char* lpszData = new char[dwSize + 1];
            ret = InternetQueryOptionA( NULL, INTERNET_OPTION_PROXY, lpszData, &dwSize );
            lpszData[dwSize] = 0;
            if( ret )
            {
                INTERNET_PROXY_INFO * info = (INTERNET_PROXY_INFO *)lpszData;
                char * proxydata =  (char *)info->lpszProxy;
                if( info->dwAccessType == 3 && proxydata && strstr( proxydata,"127.0.0.1:8888") )
                {
                    isProxy = true;
                }
            }


        }


        if( isProxy )
        {
            QNetworkProxy proxy(QNetworkProxy::ProxyType::HttpProxy,"127.0.0.1",8888);
            SetPorxy(proxy);
        }

#endif

    HttpImp*     newRequest = new HttpImp();
    m_requestList.push_back(newRequest);
    connect(newRequest, SIGNAL(S_DisposeRequest()), this, SLOT(R_DisposeRequest()));

    return newRequest;
}

void HttpManager::Get(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl)
{
    HttpImp * obj  = (HttpImp *)objparam;
    if (cmdobj->ImmediateSend() == 0)
    {
        QMutexLocker locker(&m_reqmutex);
        if (m_currCount >= m_maxPool)
        {
            obj->Requrl(strUrl);
            m_httpCmdList.append(cmdobj);
            return;
        }
    }
    m_currCount++;
    obj->SetNetWorkManager(m_manager);
    obj->AsyncGetHttpData(strUrl);
    connect(obj, SIGNAL(S_ReqFinished()), this, SLOT(R_Reqfinished()));

    Q_EMIT S_HttpReqLog(strUrl, "");
}

void HttpManager::Post(HttpCmd* cmdobj, IHttpImp* objparam, QString strUrl, QByteArray postData)
{
    HttpImp * obj  = (HttpImp *)objparam;
    if (cmdobj->ImmediateSend() == 0)
    {
        QMutexLocker locker(&m_reqmutex);
        if (m_currCount >= m_maxPool)
        {
            obj->Requrl(strUrl);
            m_httpCmdList.append(cmdobj);
            return;
        }
    }
    m_currCount++;
    obj->SetNetWorkManager(m_manager);
    obj->AsyncPostHttpData(strUrl, postData);
    connect(obj, SIGNAL(S_ReqFinished()), this, SLOT(R_Reqfinished()));

    Q_EMIT S_HttpReqLog(strUrl, QString::fromUtf8(postData));
}

void HttpManager::HtppsPost(IHttpImp* objparam, QString strUrl, QByteArray postData)
{
    HttpImp * obj  = (HttpImp *)objparam;
    obj->SetNetWorkManager(m_manager);
    obj->AsyncPostHttpsData(strUrl, postData);
    Q_EMIT S_HttpReqLog(strUrl, QString::fromUtf8(postData));
}

void HttpManager::R_DisposeRequest()
{
    QMutexLocker locker(&m_mutex);
    HttpImp*     sender = (HttpImp*)this->sender();
    foreach (HttpImp* obj, m_requestList)
    {
        if (obj == sender)
        {
            if (obj)
            {
                this->m_requestList.removeOne(obj);
                obj->deleteLater();
                obj = NULL;
            }
        }
    }
}

QNetworkAccessManager* HttpManager::InnerManager()
{
    if (m_manager == NULL)
    {
        m_manager = new QNetworkAccessManager();
    }
    return m_manager;
}

void HttpManager::R_Reqfinished()
{
    HttpCmd* obj = nullptr;
    {
        QMutexLocker locker(&m_reqmutex);
        m_currCount--;

        if (m_currCount < m_maxPool)
        {
            if (m_httpCmdList.size() > 0)
            {
                obj = m_httpCmdList.at(0);
                m_httpCmdList.removeAt(0);
            }
        }
    }

    if (obj)
    {
        obj->ImmediateSend(1);
        obj->ReReqData();
    }
}
