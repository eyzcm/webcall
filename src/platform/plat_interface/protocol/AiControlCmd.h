#pragma once
#include "protocol/tcpcmd.h"
//#include "visitormonitortcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT AiControlCmd : public TcpCmd
{
    Q_OBJECT

public:
    AiControlCmd(void);
    ~AiControlCmd(void);

public:
    virtual void MakeCmd();

public:
    void AddExtData(QString name, QString value);
    void SetExtMap(QMap<QString, QString> map);

public:
    Property_QString(ChatId, chatId);

private:
    QMap<QString, QString> m_exts;
};
