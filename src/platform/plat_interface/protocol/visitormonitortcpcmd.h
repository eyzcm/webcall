#ifndef VISITORMONITORTCPCMD_H
#define VISITORMONITORTCPCMD_H

#include "plat_interface_global.h"
#include "protocol/icmd.h"

class  PLAT_INTERFACE_EXPORT VisitorMonitorTcpCmd : public ICmd
{
    Q_OBJECT

public:
    VisitorMonitorTcpCmd();
    virtual ~VisitorMonitorTcpCmd();

    void         SendRequestToServer();
    virtual void MakeCmd();
    virtual void PreProcessMsg(TcpMessagePtr msg);

public:
    void    AddHeader(QString key, QString val);
    QString GetHeaderByKey(QString key);

    bool HasResponse() const
    {
        return m_hasResponse;
    }
    void HasResponse(bool val)
    {
        m_hasResponse = val;
    }
private slots:
    void R_Finished(TcpMessagePtr msg);

public:
    Property_QString(Body, body);
    Property_QString(Uri, uri);

private:
    bool                   m_hasResponse;
    QMap<QString, QString> m_headerList;
};

#endif // VISITORMONITORTCPCMD_H
