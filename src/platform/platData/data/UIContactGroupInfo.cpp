#include "UIContactGroupInfo.h"

UIContactGroupInfo::UIContactGroupInfo(void)
{
}

UIContactGroupInfo::UIContactGroupInfo(const UIContactGroupInfo& o)
{
    this->Memo(o.Memo());
    this->DepartmentId(o.DepartmentId());
    this->DepartmentName(o.DepartmentName());
    this->ParentId(o.ParentId());
}
/*
UIContactGroupInfo  UIContactGroupInfo::operator=(const UIContactGroupInfo & o);
{
    this->Memo(o.Memo());
    this->DepartmentId(o.DepartmentId());
    this->DepartmentName(o.DepartmentName());
    this->ParentId(o.ParentId());
}
*/

UIContactGroupInfo::UIContactGroupInfo(UIContactGroupInfo* o)
{
    if (o == NULL)
        return;
    this->Memo(o->Memo());
    this->DepartmentId(o->DepartmentId());
    this->DepartmentName(o->DepartmentName());
    this->ParentId(o->ParentId());
}

UIContactGroupInfo::~UIContactGroupInfo(void)
{
}

void UIContactGroupInfo::Memo(QString val)
{
    m_memo = val;
}

QString UIContactGroupInfo::Memo() const
{
    return m_memo;
}

void UIContactGroupInfo::DepartmentId(QString val)
{
    m_departmentId = val;
}

QString UIContactGroupInfo::DepartmentId() const
{
    return m_departmentId;
}

void UIContactGroupInfo::DepartmentName(QString val)
{
    m_departmentName = val;
}

QString UIContactGroupInfo::DepartmentName() const
{
    return m_departmentName;
}

void UIContactGroupInfo::ParentId(QString val)
{
    m_parentId = val;
}

QString UIContactGroupInfo::ParentId() const
{
    return m_parentId;
}
