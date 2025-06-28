#ifndef TCPCMD_H
#define TCPCMD_H

#include "plat_interface_global.h"

#include <protocol/icmd.h>


class  PLAT_INTERFACE_EXPORT TcpCmd : public ICmd
{
    Q_OBJECT

public:
    TcpCmd();
    virtual ~TcpCmd();
    void         SendRequestToServer();
    virtual void MakeCmd();
    virtual void PreProcessMsg(TcpMessagePtr msg);
    void         AddHeader(QString key, QString val);
    QString      GetHeaderByKey(QString key);

    bool HasResponse() const
    {
        return m_hasResponse;
    }
    void HasResponse(bool val)
    {
        m_hasResponse = val;
    }
    bool IsJsontype() const
    {
        return m_jsontype;
    }
    void IsJsontype(bool val)
    {
        m_jsontype = val;
    }

    Property_QString(Body, body);
    Property_QString(Uri, uri);
    Property_Int(RetryCount, retryCount);

private slots:
    void R_Finished(TcpMessagePtr msg);

private:
    bool                   m_hasResponse;
    bool                   m_jsontype;
    QMap<QString, QString> m_headerList;
};

#endif // TCPCMD_H
