#include "DbUiUserInfoServiceImpl.h"

DbUiUserInfoServiceImpl::DbUiUserInfoServiceImpl()
{

    m_loginData = nullptr;
    m_userInfo  = nullptr;
}

DbUiUserInfoServiceImpl::~DbUiUserInfoServiceImpl(void)
{
}

void DbUiUserInfoServiceImpl::Clear()
{
    if (m_userInfo)
    {
        m_userInfo->deleteLater();
        m_userInfo = nullptr;
    }

    if (m_loginData)
    {
        m_loginData->deleteLater();
        m_loginData = nullptr;
    }
}

void DbUiUserInfoServiceImpl::UserInfo(UIUserInfo* info)
{
    m_userInfo = info;
}

void DbUiUserInfoServiceImpl::LoginData(UILoginData* info)
{
    m_loginData = info;
}

UIUserInfo* DbUiUserInfoServiceImpl::UserInfo() const
{
    return m_userInfo;
}

UIUserInfo* DbUiUserInfoServiceImpl::CreateInfo(UIUserInfo* info)
{
    UIUserInfo* tmpinfo = new UIUserInfo(info);
    return tmpinfo;
}

UILoginData* DbUiUserInfoServiceImpl::CreateLoginInfo(UILoginData* info)
{
    UILoginData* tmpinfo = new UILoginData(info);
    return tmpinfo;
}

QString DbUiUserInfoServiceImpl::LockPassword()
{
    return m_lockPassword;
}

void DbUiUserInfoServiceImpl::LockPassword(QString password)
{
    m_lockPassword = password;
}

void DbUiUserInfoServiceImpl::SetNewToken(QString token)
{
    if (m_userInfo)
        m_userInfo->Token(token);
}
