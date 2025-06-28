#ifndef colleagueService_H
#define colleagueService_H
#include <QObject>
#include "plat_permission_global.h"
#include "data/PlatDepartmentInfo.h"
#include "data/PlatUserInfo.h"
#include "session/ColleagueMgrInterface.h"

#include <QList>
#include "utils/UISingleton.h"

class PLAT_PERMISSION_EXPORT colleagueService: public ColleagueMgrInterface,public UISingleton<colleagueService>
{
    friend class UISingleton<colleagueService>;

    Q_OBJECT
public:
    colleagueService();

    void                    SetDepartmentList(const QString &body);
    void                    SetUserInfoList(const QString &body,int pageno);
    int                     GetMaxPage();
    QList<PlatDepartmentInfo *> GetDepartmentList();
    QList<PlatUserInfo *> GetUserInfoList();
    bool  IsLoadUserInfoEnd();
    void  IsLoadUserInfoEnd(bool val);

private:

    QList<PlatDepartmentInfo *>   m_departmentList;
    QList<PlatUserInfo *>   m_userInfoList;
    int  m_maxUserInfoPage;
    bool  m_IsLoadUserInfoEnd;


};

#define gcolleagueSerice colleagueService::GetInstance()

#endif // colleagueService_H
