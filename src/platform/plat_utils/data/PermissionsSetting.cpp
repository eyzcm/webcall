#include "PermissionsSetting.h"

PermissionsSetting::PermissionsSetting(void)
{
}

PermissionsSetting::~PermissionsSetting(void)
{
}

bool PermissionsSetting::HasPermissionByKey(const QString& strKey)
{
    return m_permissionList.find(strKey) != m_permissionList.end();
}

int PermissionsSetting::GetPermissionData(const QString& strKey)
{
    if (m_permissionList.find(strKey) != m_permissionList.end())
    {
        return m_permissionList[strKey];
    }
    return 0;
}

QString PermissionsSetting::GetAiPermission()
{
    QStringList list;
    for (auto itor = m_permissionList.begin(); itor != m_permissionList.end(); itor++)
    {
        QString key = itor.key();
        if (key.indexOf("c_ai_") == 0)
            list.append(itor.key());
    }
    return list.join(",");
}
