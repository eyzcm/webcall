#include "UIUserInfo.h"

UIUserInfo::UIUserInfo(void)
{
    this->m_userRole = E_ADMIN;
    m_aiFlag         = 1;
}

UIUserInfo::UIUserInfo(const UIUserInfo& info)
{
    this->CompanyId(info.CompanyId());
    this->NickName(info.NickName());
    this->RealName(info.RealName());
    this->Token(info.Token());
    this->UserId(info.UserId());
    this->LoginStatus(info.LoginStatus());
    this->UserRole(info.UserRole());
    this->AiFlag(info.AiFlag());
    this->DepartmentId(info.DepartmentId());
}

UIUserInfo::UIUserInfo(UIUserInfo* pinfo)
{
    if (pinfo == NULL)
        return;
    this->CompanyId(pinfo->CompanyId());
    this->NickName(pinfo->NickName());
    this->RealName(pinfo->RealName());
    this->Token(pinfo->Token());
    this->UserId(pinfo->UserId());
    this->LoginStatus(pinfo->LoginStatus());
    this->UserRole(pinfo->UserRole());
    this->AiFlag(pinfo->AiFlag());
    this->DepartmentId(pinfo->DepartmentId());
}

UIUserInfo::~UIUserInfo(void)
{
}

void UIUserInfo::NickName(QString val)
{
    m_nickName = val;
}

QString UIUserInfo::NickName() const
{
    return m_nickName;
}

void UIUserInfo::Token(QString val)
{
    m_token = val;
}

QString UIUserInfo::Token() const
{
    return m_token;
}

void UIUserInfo::UserId(QString val)
{
    m_userId = val;
}

QString UIUserInfo::UserId() const
{
    return m_userId;
}

void UIUserInfo::RealName(QString val)
{
    m_realName = val;
}

QString UIUserInfo::RealName() const
{
    return m_realName;
}

void UIUserInfo::CompanyId(QString val)
{
    m_companyId = val;
}

QString UIUserInfo::CompanyId() const
{
    return m_companyId;
}

QString UIUserInfo::DepartmentId() const
{
    return m_departmentId;
}

void UIUserInfo::DepartmentId(QString val)
{
    m_departmentId = val;
}

void UIUserInfo::LoginStatus(ELOGINSTATUS val)
{
    m_loginStatus = val;
}

ELOGINSTATUS UIUserInfo::LoginStatus() const
{
    return m_loginStatus;
}

QString UIUserInfo::GetDisplayName()
{
    QString ret = "";
    ret         = RealName();
    if (ret.isEmpty())
    {
        ret = NickName();
    }
    if (ret.isEmpty())
    {
        ret = UserId();
    }
    if (ret.isEmpty())
    {
        Q_ASSERT(false);
    }
    return ret;
}

EUSERROLE UIUserInfo::UserRole() const
{
    return m_userRole;
}

void UIUserInfo::UserRole(EUSERROLE val)
{
    m_userRole = val;
}

int UIUserInfo::AiFlag() const
{
    return m_aiFlag;
}

void UIUserInfo::AiFlag(int val)
{
    m_aiFlag = val;
}
