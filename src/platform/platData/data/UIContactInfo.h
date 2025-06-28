#pragma once
#include "utils/plat_EnumDefine.h"

#include <QMetaType>
#include <QObject>
#include "platdata_global.h"


class PLATDTA_EXPORT UIContactInfo : public QObject
{
    Q_OBJECT

public:
    UIContactInfo(void);
    UIContactInfo(UIContactInfo* info);
    UIContactInfo(const UIContactInfo& info);
    UIContactInfo & operator =(const UIContactInfo & info);
    ~UIContactInfo(void);

public:
    QString Name() const;
    void    Name(QString val);

    QString UId() const;
    void    UId(QString val);

    QString NickName() const;
    void    NickName(QString val);

    QString UserId() const;
    void    UserId(QString val);

    int  AiFlag() const;
    void AiFlag(int val);

    EPRESENCESTATUS Status() const;
    void            Status(EPRESENCESTATUS val);

    CUSTOMER_LOGIN_TYPE LoginType() const;
    void                LoginType(CUSTOMER_LOGIN_TYPE val);
    QString             DepartmentId() const;
    void                DepartmentId(QString val);

    QString DepartmentName() const;
    void    DepartmentName(QString val);

    QString DisplayInnerName() const
    {
        QString ret = m_name;
        if (ret.isEmpty())
        {
            ret = m_userId;
        }
        return ret;
    }
    QString DisplayOutterName() const
    {
        QString ret = m_nickName;
        if (ret.isEmpty())
        {
            ret = m_userId;
        }
        return ret;
    }

private:
    QString             m_uId;
    QString             m_name;
    QString             m_nickName;
    QString             m_userId;
    EPRESENCESTATUS     m_status;
    CUSTOMER_LOGIN_TYPE m_loginType;
    QString             m_departmentId;
    QString             m_departmentName;
    int                 m_aiFlag; // 1表示人工客服 2表示ai客服
};

Q_DECLARE_METATYPE(UIContactInfo)
