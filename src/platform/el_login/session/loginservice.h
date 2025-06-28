#ifndef LOGINSERVICE_H
#define LOGINSERVICE_H

#include "utils/Plat_EnumDefine.h"
#include <QObject>
class ProtocolWork;

class LoginService : public QObject
{
    Q_OBJECT

public:
    LoginService();
    ~LoginService();
    void ReLogin();
signals:
    void S_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
    void S_MainTcpNetWorkError(ELOGINSTATUS loginStatus, int errorCode, QString msg);
    void S_Relogin();


};

#endif // LOGINSERVICE_H
