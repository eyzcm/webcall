#pragma once
#include "QMap"

#include <QList>
#include <QString>

class  PermissionsSetting
{
public:
    PermissionsSetting(void);
    ~PermissionsSetting(void);

    void AddPermission(QString permission, int permdata = 0)
    {
        if (m_permissionList.find(permission) == m_permissionList.end())
        {
            m_permissionList.insert(permission, permdata);
        }
        else
            m_permissionList[permission] = permdata;
    }

    bool    HasPermissionByKey(const QString& strKey);
    int     GetPermissionData(const QString& strKey);
    QString GetAiPermission();

private:
    QMap<QString, int> m_permissionList;
};

//"permissions": ["visitorMonitor",
//	"chatMonitor",
//	"report",
//	"visitorAnalyse",
//	"console",
//	"visitorCard",
//	"chatRecord",
//	"leaveMessage",
//	"registryForm",
//	"miniHTML","CRM","colleague","transchat","summaryLabel",
//	"CARD_ALLOCATION",
//	"WORK_ORDER",
//	"AI"
