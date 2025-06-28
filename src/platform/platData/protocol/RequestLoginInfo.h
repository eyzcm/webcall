#ifndef REQUESTLOGININFO_H
#define REQUESTLOGININFO_H

#include "protocol/IRequest.h"
#include "platdata_global.h"


class PLATDTA_EXPORT RequestLoginInfo : public IRequest
{
    Q_OBJECT
public:
    RequestLoginInfo(void);
    ~RequestLoginInfo(void);
    RequestObjectName("RequestLoginInfo");
    virtual QString MakeXml();

public:
    QString UserName() const
    {
        return m_userName;
    }
    void UserName(QString val)
    {
        m_userName = val;
    }

    QString Password() const
    {
        return m_password;
    }
    void Password(QString val)
    {
        m_password = val;
    }

    QString Port() const
    {
        return m_port;
    }
    void Port(QString val)
    {
        m_port = val;
    }

    QString ConnectService() const
    {
        return m_connectService;
    }
    void ConnectService(QString val)
    {
        m_connectService = val;
    }

private:
    QString m_userName;
    QString m_password;
    QString m_connectService;
    QString m_port;
};
#endif // REQUESTLOGININFO_H
