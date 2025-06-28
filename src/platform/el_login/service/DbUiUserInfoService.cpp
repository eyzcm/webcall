#include "DbUiUserInfoService.h"
#include "service/imp/DbUiUserInfoServiceImpl.h"

DbUiUserInfoService::DbUiUserInfoService(DbUiService* mainService)
{
    m_service = new DbUiUserInfoServiceImpl();
}

DbUiUserInfoService::~DbUiUserInfoService(void)
{
    if (m_service)
        delete m_service;
}

void DbUiUserInfoService::Clear()
{
    QMutexLocker locker(&m_mutex);
    return m_service->Clear();
}

void DbUiUserInfoService::UserInfo(UIUserInfo* info)
{
    QMutexLocker locker(&m_mutex);
    return m_service->UserInfo(info);
}

void DbUiUserInfoService::LoginData(UILoginData* info)
{
    QMutexLocker locker(&m_mutex);
    return m_service->LoginData(info);
}

UIUserInfo* DbUiUserInfoService::UserInfo()
{
    QMutexLocker locker(&m_mutex);
    return m_service->UserInfo();
}

UIUserInfo* DbUiUserInfoService::CreateInfo(UIUserInfo* info)
{
    return m_service->CreateInfo(info);
}

UILoginData* DbUiUserInfoService::CreateLoginInfo(UILoginData* info)
{
    return m_service->CreateLoginInfo(info);
}

QString DbUiUserInfoService::LockPassword()
{
    return m_service->LockPassword();
}
void DbUiUserInfoService::LockPassword(QString password)
{
    return m_service->LockPassword(password);
}

void DbUiUserInfoService::SetNewToken(QString token)
{
    return m_service->SetNewToken(token);
}

UILoginData* DbUiUserInfoService::LoginData() const
{
    return m_service->LoginData();
}

QString      DbUiUserInfoService::ServiceId()
{
    return "DbUiUserInfoInterface";
}

