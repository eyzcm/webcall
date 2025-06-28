#include "usercenterhttpagent.h"

#include <protocol/HttpCmd.h>
#include "session/el_sessionservice.h"
#include "service/permissionservice.h"
#include "service/ColleagueService.h"

UserCenterHttpAgent::UserCenterHttpAgent() : QObject()
{
    m_currentUserPage = 1;

}

UserCenterHttpAgent* UserCenterHttpAgent::m_instance = nullptr;

UserCenterHttpAgent* UserCenterHttpAgent::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new UserCenterHttpAgent;
    }
    return m_instance;
}

UserCenterHttpAgent::~UserCenterHttpAgent()
{


}

void UserCenterHttpAgent::Init(QString url,QString token )
{
    m_baseUrl = url;
	m_token = token;
}

void UserCenterHttpAgent::GetProductList()
{

    HttpCmd* cmd = new HttpCmd;
    cmd->SetHeader("Authorization", m_token);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetProductListFinished(QString, QString, QString)));

    QString ret = MakeUri(m_baseUrl,"product-list");
    cmd->Uri(ret);
    cmd->Get();
}


void UserCenterHttpAgent::GetMenuList()
{

    HttpCmd* cmd = new HttpCmd;
    cmd->SetHeader("Authorization", m_token);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetMenuListFinished(QString, QString, QString)));

    QString ret = MakeUri(m_baseUrl,"menu-list");
    cmd->Uri(ret);
    cmd->Get();

}

QString UserCenterHttpAgent::MakeUri(QString url, QString cmdTag)
{
       QString cmdstr = "";
       cmdstr = QString("%1/auth-eupms/per/res/%2?appCode=im-client").arg(url).arg(cmdTag);
       return cmdstr;


}


QString UserCenterHttpAgent::MakeClientUri(QString url, QString cmdTag)
{
       QString cmdstr = "";
       cmdstr = QString("%1/auth-eupms/client/%2").arg(url).arg(cmdTag);
       return cmdstr;


}

void UserCenterHttpAgent::R_GetProductListFinished(QString code, QString reason, QString body )
{


}

void UserCenterHttpAgent::R_GetMenuListFinished(QString code, QString reason, QString body)
{
    gPermissionSerice->SetFunMenuList(body);

}


void UserCenterHttpAgent::GetDeptList()
{

    HttpCmd* cmd = new HttpCmd;
    cmd->SetHeader("Authorization", m_token);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetDeptListFinished(QString, QString, QString)));

    QString ret = MakeClientUri(m_baseUrl,"dept/list");
    cmd->Uri(ret);
    cmd->Get();

}

void UserCenterHttpAgent::R_GetDeptListFinished(QString code, QString reason, QString body)
{
    gcolleagueSerice->SetDepartmentList(body);
}



void UserCenterHttpAgent::GetUserList( int pageNo,int pageSize  )
{
    m_currentUserPage = pageNo;
    HttpCmd* cmd = new HttpCmd;
    cmd->SetHeader("Authorization", m_token);
    connect(cmd, SIGNAL(S_Finished(QString, QString, QString)), this, SLOT(R_GetUserListFinished(QString, QString, QString)));

    QString ret = MakeClientUri(m_baseUrl,QString("user/list?pageNo=%1&pageSize=%2").arg(pageNo).arg(pageSize));
    cmd->Uri(ret);
    cmd->Get();

}

void UserCenterHttpAgent::R_GetUserListFinished(QString code, QString reason, QString body)
{

    gcolleagueSerice->SetUserInfoList(body,m_currentUserPage);
    m_currentUserPage++;
    if(m_currentUserPage <= gcolleagueSerice->GetMaxPage() )
        GetUserList(m_currentUserPage);
    else
        gcolleagueSerice->IsLoadUserInfoEnd(true);

}

