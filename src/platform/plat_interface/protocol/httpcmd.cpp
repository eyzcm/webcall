#include "httpcmd.h"
#include "utils/UtilsHelper.h"
#include "protocol/platprotocolservice.h"

HttpCmd::HttpCmd() : QObject()
{
    m_ImmediateSend = 0;
    m_reqType       = 0;
    m_time          = nullptr;
    CmdID(UtilsHelper::GetUUid());
    setHttpManager(gHttpManager);
}

HttpCmd::~HttpCmd()
{
    deleteTime();
}

void HttpCmd::ReInit()
{
    m_ImmediateSend = 0;
    m_reqType       = 0;
    deleteTime();
    m_body.clear();
    m_headerList.clear();
    m_cmdDataList.clear();
    qDeleteAll(m_cmdCacheData);
}

void HttpCmd::ImmediateSend(int type)
{
    m_ImmediateSend = type;
}

int HttpCmd::ImmediateSend()
{
    return m_ImmediateSend;
}

void HttpCmd::Get()
{
    IHttpImp *obj = gHttpManager->CreateHttpRequest();
      connect(obj, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_Finished(QString, QString, QString)));
      connect(obj, SIGNAL(S_SourceFinished(QString, QString, QByteArray)), this, SLOT(R_SourceFinished(QString, QString, QByteArray)));
      //设置自定义头
      // todo shizhenyu
      for (auto itor = m_headerList.begin(); itor != m_headerList.end(); itor++)
      {
          obj->SetHeader(itor.key(), itor.value());
      }
    m_reqType = 0;
    timeoutRetry();
    m_objList.append(obj);
    m_HttpManager->Get(this, obj,Uri());
    //Q_EMIT m_HttpManager->S_HttpReqLog(CmdID() + " get \n" + Uri(), "");

}

void HttpCmd::Post(const QByteArray &data)
{
    IHttpImp *obj = gHttpManager->CreateHttpRequest();
      connect(obj, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_Finished(QString, QString, QString)));
      connect(obj, SIGNAL(S_SourceFinished(QString, QString, QByteArray)), this, SLOT(R_SourceFinished(QString, QString, QByteArray)));
	  for (auto itor = m_headerList.begin(); itor != m_headerList.end(); itor++)
	  {
		  obj->SetHeader(itor.key(), itor.value());
	  }
    m_reqType = 1;
    m_body    = data;
    timeoutRetry();
    m_objList.append(obj);
    m_HttpManager->Post(this,obj, Uri(), data);
    //Q_EMIT m_HttpManager->S_HttpReqLog(CmdID() + " post \n" + Uri(), data);

}


void HttpCmd::HttpsPost(const QByteArray &data, const QString &filePath)
{
    Post(data);

}

void HttpCmd::setHttpManager(Plat_HttpManagerInterface *hm)
{
    m_HttpManager = hm;
    if (!m_HttpManager)
    {
        m_HttpManager = gHttpManager;
    }
}

void HttpCmd::ReReqData()
{
    CmdID(UtilsHelper::GetUUid()); //重新请求后,忽略之前的请求回复内容

    if (m_reqType == 0)
    {
        Get();
    }
    else
    {
        Post(m_body);
    }
}

void HttpCmd::SetHeader(const QString &key, const QString &val)
{
    m_headerList.insert(key, val);
}

void HttpCmd::AddCmdData(const QString &key, const QString &val)
{
    m_cmdDataList.insert(key, val);
}

QString HttpCmd::GetCmdDataByKey(const QString &key)
{
    if (m_cmdDataList.find(key) == m_cmdDataList.end())
    {
        return "";
    }
    else
    {
        return m_cmdDataList[key];
    }
}

void HttpCmd::DoFinished(QString code, QString reason, QString rsp)
{
    deleteTime();
    Q_EMIT S_Finished(code, reason, rsp);
    //Q_EMIT m_HttpManager->S_HttpReqLog(CmdID() + " error Finished \n", rsp);
}

void HttpCmd::timeoutRetry()
{
    QString timeout = GetCmdDataByKey("timeout");
    if(!timeout.isEmpty() && timeout.toInt() > 0 && m_time == nullptr) //m_time == nullptr 防止多次重试
    {
        m_time = new QTimer();
        m_time->setSingleShot(true);
        connect(m_time, &QTimer::timeout, this, &HttpCmd::R_timeoutRetry);
        m_time->start(timeout.toInt() * 1000);
    }
}

void HttpCmd::deleteTime()
{
    if(m_time)
    {
        m_time->stop();
        m_time->deleteLater();
        m_time = nullptr;
    }
}

void HttpCmd::AddCacheByKey(const QString &key, void *data)
{
    m_cmdCacheData.insert(key, data);
}

void *HttpCmd::GetCacheByKey(const QString &key)
{
    if (m_cmdCacheData.find(key) == m_cmdCacheData.end())
    {
        return nullptr;
    }
    else
    {
        return m_cmdCacheData[key];
    }
}

QString HttpCmd::GetUriWithBody()
{
    QString uri = Uri();
    QString str = QString(m_body);
    if (str != "")
        if (uri.indexOf("?") < 0)
            uri += "?";
        else
            uri += "&";
    uri += str;
    return uri;
}

void HttpCmd::R_SourceFinished(QString code, QString reason, QByteArray body)
{

    {
        Q_EMIT S_SourceFinished(code, reason, body); //下载文件时才有效,body文件流内容
        // Q_EMIT m_HttpManager->S_HttpReqLog(CmdID() + " Finished \n" + Uri(), body);
    }
}

void HttpCmd::R_Finished(QString code, QString reason, QString rsp)
{

	if (m_time)
	{
		deleteTime();
		foreach(IHttpImp * obj, m_objList)
		{
			if (sender() != obj)
			{
				disconnect(obj, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_Finished(QString, QString, QString)));
				disconnect(obj, SIGNAL(S_SourceFinished(QString, QString, QByteArray)), this, SLOT(R_SourceFinished(QString, QString, QByteArray)));

			}
		}
	}
    Q_EMIT S_Finished(code, reason, rsp);
        //Q_EMIT m_HttpManager->S_HttpReqLog(CmdID() + " Finished \n", rsp);
    
}

void HttpCmd::R_DownLoadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    //if (myId == CmdID())
    {
        Q_EMIT S_DownLoadProgress(bytesReceived, bytesTotal);
    }
}

void HttpCmd::R_timeoutRetry()
{
    ReReqData();
}
