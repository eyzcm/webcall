#ifndef ICMD_H
#define ICMD_H

#include "utils/DefineHelper.h"
#include "utils/Plat_EnumDefine.h"
#include "tcpmessage.h"
#include <QMap>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT ICmd : public QObject
{
    Q_OBJECT

public:
    ICmd();
    virtual ~ICmd();
    virtual void SendRequestToServer() = 0;
    virtual void MakeCmd()             = 0;
signals:
    void S_Finished(TcpMessagePtr msg);

private:
};

#endif // ICMD_H
