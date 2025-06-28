#include "UIContactInfo.h"

UIContactInfo::UIContactInfo(void)
{
    m_aiFlag    = 1;
    m_status    = E_NONE;
    m_loginType = CUSTOMER_LOGIN_CLIENT;
}

UIContactInfo::UIContactInfo(const UIContactInfo& info)
{
    this->Name(info.Name());
    this->NickName(info.NickName());
    this->UserId(info.UserId());
    this->Status(info.Status());
    this->DepartmentId(info.DepartmentId());
    this->DepartmentName(info.DepartmentName());
    this->AiFlag(info.AiFlag());
    this->LoginType(info.LoginType());
}

UIContactInfo & UIContactInfo::operator =(const UIContactInfo & info)
{
    this->Name(info.Name());
    this->NickName(info.NickName());
    this->UserId(info.UserId());
    this->Status(info.Status());
    this->DepartmentId(info.DepartmentId());
    this->DepartmentName(info.DepartmentName());
    this->AiFlag(info.AiFlag());
    this->LoginType(info.LoginType());
    return *this;
}

UIContactInfo::UIContactInfo(UIContactInfo* pinfo)
{
    if (pinfo == NULL)
    {
        this->AiFlag(1);
        m_loginType = CUSTOMER_LOGIN_CLIENT;
    }
    else
    {
        this->Name(pinfo->Name());
        this->NickName(pinfo->NickName());
        this->UserId(pinfo->UserId());
        this->Status(pinfo->Status());
        this->DepartmentId(pinfo->DepartmentId());
        this->DepartmentName(pinfo->DepartmentName());
        this->AiFlag(pinfo->AiFlag());
        this->LoginType(pinfo->LoginType());
    }
}
UIContactInfo::~UIContactInfo(void)
{
}

void UIContactInfo::Name(QString val)
{
    m_name = val;
}

QString UIContactInfo::Name() const
{
    return m_name;
}

void UIContactInfo::NickName(QString val)
{
    m_nickName = val;
}

QString UIContactInfo::NickName() const
{
    return m_nickName;
}

void UIContactInfo::UserId(QString val)
{
    m_userId = val;
}

QString UIContactInfo::UserId() const
{
    return m_userId;
}

void UIContactInfo::LoginType(CUSTOMER_LOGIN_TYPE val)
{
    m_loginType = val;
}

CUSTOMER_LOGIN_TYPE UIContactInfo::LoginType() const
{
    return m_loginType;
}

void UIContactInfo::Status(EPRESENCESTATUS val)
{
    m_status = val;
}

EPRESENCESTATUS UIContactInfo::Status() const
{
    return m_status;
}

void UIContactInfo::DepartmentId(QString val)
{
    m_departmentId = val;
}

QString UIContactInfo::DepartmentId() const
{
    return m_departmentId;
}

void UIContactInfo::DepartmentName(QString val)
{
    m_departmentName = val;
}

QString UIContactInfo::DepartmentName() const
{
    return m_departmentName;
}

int UIContactInfo::AiFlag() const
{
    return m_aiFlag;
}

void UIContactInfo::AiFlag(int val)
{
    m_aiFlag = val;
}

QString UIContactInfo::UId() const
{
    return m_uId;
}
void    UIContactInfo::UId(QString val)
{
    m_uId = val;
}
