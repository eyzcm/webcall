#pragma once
#include "ElVmState.h"

class ElVmStatusProcess;

class ElVmConnectingState : public ElVmState
{

    ElVmStatusProcess* m_process;

public:
    ElVmConnectingState(ElVmStatusProcess* process);
    ~ElVmConnectingState(void);

    virtual bool Connect();

    virtual bool TcpLogin(QByteArray msg);

    virtual bool SendTcpMessage(QByteArray msg);

    virtual bool VMConnect();

    virtual bool VMLogin();

    virtual bool VMSendMessage();

    virtual bool ReConnect();

    virtual bool LoginOut();

    virtual bool InitState();

    virtual bool VMAuth(QByteArray msg);
};
