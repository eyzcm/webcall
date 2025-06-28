#pragma once
#include "utils/Plat_EnumDefine.h"
#include <QObject>
#include "platdata_global.h"

#define bool bool

typedef enum
{
    CONNECT = 0,
    CONNECTING,
    LOGIN,
    LOGINING,
    LOGINOK,
    END
} ELTCPlLOGINSTATUS;

class PLATDTA_EXPORT ElConnectState
{

public:
    virtual bool InitState();
    virtual bool Connect();

    virtual bool TcpLogin(QByteArray msg);

    virtual bool SendTcpMessage(QByteArray msg);

    virtual bool VMConnect();

    virtual bool VMLogin();

    virtual bool VMSendMessage();

    virtual bool ReConnect();

    virtual bool LoginOut();
};
