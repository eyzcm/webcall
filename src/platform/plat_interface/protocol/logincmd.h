#ifndef LOGINCMD_H
#define LOGINCMD_H


#include "tcpcmd.h"
#include "plat_interface_global.h"

class  PLAT_INTERFACE_EXPORT LoginCmd : public TcpCmd
{
    Q_OBJECT

public:
    LoginCmd();
    ~LoginCmd();
    void IsForce( bool forcelogin);

    void Init(QString userName, QString password, QString loginUrl, QString port);
    void LoginType(int type);


    void            LoginStatus(EPRESENCESTATUS status);
    EPRESENCESTATUS LoginStatus();

    void IsFriendData(int friendType);

    int IsFriendData();
    void Version(QString ver){m_version = ver;};
    virtual void MakeCmd();
    virtual void PreProcessMsg(TcpMessagePtr msg);

private slots:
    void R_connected();

private:
    QString         m_version;
    QString         m_userName;
    QString         m_passWord;
    QString         m_loginUrl;
    QString         m_port;
    EPRESENCESTATUS m_loginStatus;
    int             m_isFriendData;
    int             m_loginType; // 0,正常用户密码登录 1.微信登录
    bool            m_force;   //false,不强制踢掉云端，true.强制踢掉云端
};

#endif // LOGINCMD_H
