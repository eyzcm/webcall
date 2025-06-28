#ifndef EventCustomCmd_H
#define EventCustomCmd_H


#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT EventCustomCmd : public TcpCmd
{
    Q_OBJECT

public:
    EventCustomCmd(QString chatId);
    ~EventCustomCmd();

    virtual void MakeCmd();

    void AddMap(QString key, QString val);

    // to为目标客服
    Property_QString(MsgTo, msgTo);
    Property_QString(Sender, sender);
    Property_QString(SubType, subType);

private:
    QString m_chatId;

    QMap<QString, QString> m_map;
};

#endif // EventCustomCmd_H
