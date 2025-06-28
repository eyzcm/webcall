#pragma once
#include "utils/Plat_EnumDefine.h"
#include <QObject>

#define bool bool

typedef enum
{
    VMCONNECT = 0,
    VMCONNECTING,
    VMAUTH,
    VMLOGIN,

    VMLOGINING,
    VMLOGINOK,
    LOGINOUT,
    VM_END
} ElVmStatus;

class ElVmState
{

public:
    virtual bool InitState();
    virtual bool VMLogin();
    virtual bool VMAuth(QByteArray msg);

    virtual bool VMLogin(QByteArray msg);

    virtual bool VMSendMessage(QByteArray msg);

    virtual bool VMConnect();

    virtual bool ReConnect();

    virtual bool LoginOut();
};
