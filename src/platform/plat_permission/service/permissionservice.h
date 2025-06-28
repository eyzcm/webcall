#ifndef PERMISSIONSERVICE_H
#define PERMISSIONSERVICE_H
#include <QObject>
#include "plat_permission_global.h"
#include "data/UIMenuDataInfo.h"
#include <QList>
#include "utils/UISingleton.h"

class PLAT_PERMISSION_EXPORT PermissionService: public QObject,public UISingleton<PermissionService>
{
    friend class UISingleton<PermissionService>;

    Q_OBJECT
public:
    PermissionService();

    void                    SetFunMenuList(const QString &body);
    QList<UIMenuDataInfo *> GetFunMenuList();

private:
    static bool CompareMenuData(const UIMenuDataInfo *info1, const UIMenuDataInfo *info2);

    QList<UIMenuDataInfo *>   m_menuInfoList;



};

#define gPermissionSerice PermissionService::GetInstance()

#endif // PERMISSIONSERVICE_H
