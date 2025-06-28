#pragma once

#include <QMetaType>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT UIContactGroupInfo : public QObject
{
    Q_OBJECT

public:
    UIContactGroupInfo(void);
    UIContactGroupInfo(const UIContactGroupInfo& o);
    UIContactGroupInfo(UIContactGroupInfo* o);
    ~UIContactGroupInfo(void);
    UIContactGroupInfo  & operator=(const UIContactGroupInfo & o){
        this->Memo(o.Memo());
    this->DepartmentId(o.DepartmentId());
    this->DepartmentName(o.DepartmentName());
    this->ParentId(o.ParentId());
        return *this;
    }

public:
    QString Memo() const;
    void    Memo(QString val);

    QString DepartmentId() const;
    void    DepartmentId(QString val);

    QString DepartmentName() const;
    void    DepartmentName(QString val);

    QString ParentId() const;
    void    ParentId(QString val);

private:
    QString m_memo;
    QString m_departmentId;
    QString m_departmentName;
    QString m_parentId;
};
Q_DECLARE_METATYPE(UIContactGroupInfo)
