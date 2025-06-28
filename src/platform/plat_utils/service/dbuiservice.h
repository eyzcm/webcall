#ifndef DBSERVICE_H
#define DBSERVICE_H

#include "service/DbUiUserInfoInterface.h"


#include <QObject>
#include <qhash.h>

class  DbUiService
{
public:
    DbUiService();
    ~DbUiService();
    static DbUiService          *GetInstance1();
    static void                  Release();
    DbUiUserInfoInterface         *UserInfoService();

    void AddService(  DbUiBaseService * service  );


private:
    static DbUiService          *m_server;
    DbUiUserInfoInterface         *m_userInfoService;
};

#define gUIUserInfoManager DbUiService::GetInstance1()->UserInfoService()


#endif // DBSERVICE_H
