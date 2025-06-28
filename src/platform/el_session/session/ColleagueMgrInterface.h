#ifndef ColleagueMgrInterface_H
#define ColleagueMgrInterface_H
#include <QWidget>
#include "el_session_global.h"
class PlatDepartmentInfo;
class PlatUserInfo;

class EL_SESSION_EXPORT ColleagueMgrInterface: public QObject
{
	Q_OBJECT
public:
    ColleagueMgrInterface();

    virtual void                    SetDepartmentList(const QString &body) = 0;
    virtual void                    SetUserInfoList(const QString &body,int pageno) = 0;
    virtual int                     GetMaxPage()= 0;
    virtual QList<PlatDepartmentInfo *> GetDepartmentList()= 0;
    virtual QList<PlatUserInfo *> GetUserInfoList()= 0;
    virtual bool  IsLoadUserInfoEnd()= 0;
    virtual void  IsLoadUserInfoEnd(bool val)= 0;


signals:
    void S_LoadUserInfoEnd( );


};

#endif // ColleagueMgrInterface_H
