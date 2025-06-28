#pragma once
#include "utils/Plat_EnumDefine.h"

#include <QObject>

class  UIUserInfo : public QObject
{
    Q_OBJECT
public:
    UIUserInfo(void);
    UIUserInfo(const UIUserInfo& info);
    UIUserInfo(UIUserInfo* info);
    ~UIUserInfo(void);

public:
    QString      NickName() const;
    void         NickName(QString val);
    QString      Token() const;
    void         Token(QString val);
    QString      UserId() const;
    void         UserId(QString val);
    QString      RealName() const;
    void         RealName(QString val);
    QString      CompanyId() const;
    void         CompanyId(QString val);
    QString      DepartmentId() const;
    void         DepartmentId(QString val);
    ELOGINSTATUS LoginStatus() const;
    void         LoginStatus(ELOGINSTATUS val);
    QString      GetDisplayName();
    int          AiFlag() const;
    void         AiFlag(int val);

    EUSERROLE UserRole() const;
    void      UserRole(EUSERROLE val);

private:
    QString      m_nickName;
    QString      m_companyId;
    QString      m_departmentId;
    QString      m_token;
    QString      m_userId;
    QString      m_realName;
    ELOGINSTATUS m_loginStatus;
    EUSERROLE    m_userRole;
    int          m_aiFlag; // 0表示人工客服 1表示ai客服
};
