#include "httpplatcacheagent.h"
#include <QTimer>
#include "httpplatclientapiagent.h"
#include "utils/utilshelper.h"
#include "session/el_sessionservice.h"



HttpPlatCacheAgent::HttpPlatCacheAgent(QObject *parent) : QObject(parent)
{
    m_timecount                         = 1;
    m_apiColleagueVersion              = "1.0";
    m_dataColleagueVersion              = "1.0";
}


QString HttpPlatCacheAgent::DataColleagueVersion() const
{
    return m_dataColleagueVersion;
}
void HttpPlatCacheAgent::DataColleagueVersion(QString val)
{
    m_dataColleagueVersion = val;
}


void HttpPlatCacheAgent::GetColleagueList()
{
    QString body = GetCacheDataByKey("colleaguelist");
    if (body.isEmpty())
        HttpPlatClientApiAgent::GetInstance()->GetColleagueList();
    else
    {
        HttpPlatClientApiAgent::GetInstance()->R_GetColleagueListFinished("200", "ok", body);
        //延时获取加载,
        QTimer* time = new QTimer();
        time->setSingleShot(true);
        time->setInterval(10 * 1000 * m_timecount++);

        time->start();
        connect(time, SIGNAL(timeout()), this, SLOT(R_GetColleagueList()));
    }
}

void HttpPlatCacheAgent::R_GetColleagueList()
{
    HttpPlatClientApiAgent::GetInstance()->GetColleagueList();
}


QString HttpPlatCacheAgent::ApiColleagueVersion() const
{
    return m_apiColleagueVersion;
}
void HttpPlatCacheAgent::ApiColleagueVersion(QString val)
{
    m_apiColleagueVersion = val;
}


QString HttpPlatCacheAgent::GetCacheDataByKey(QString key)
{
    QString fileName = QString("%1/%2.conf").arg(UtilsHelper::GetHttpCacheDataDir(m_userId.isEmpty()?gSessionService->UserInfo()->UserId():m_userId)).arg(key);
    QString val      = UtilsHelper::LoadFile(fileName);
    return val;
}

void HttpPlatCacheAgent::SaveCacheDataByKey(QString key, QString value)
{
    QString fileName = QString("%1/%2.conf").arg(UtilsHelper::GetHttpCacheDataDir(m_userId.isEmpty()?gSessionService->UserInfo()->UserId():m_userId)).arg(key);
    UtilsHelper::SaveFile(fileName, value);
}


QString HttpPlatCacheAgent::BodyColleague() const
{
    return m_bodyColleague;
}
void HttpPlatCacheAgent::BodyColleague(QString val)
{
    m_bodyColleague = val;
}

