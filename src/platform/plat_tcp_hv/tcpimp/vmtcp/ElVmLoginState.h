#pragma once
#include "ElVmState.h"

class ElVmStatusProcess;

class ElVmLoginState : public ElVmState
{

    ElVmStatusProcess* m_process;

public:
    ElVmLoginState(ElVmStatusProcess* process);
    ~ElVmLoginState(void);

    virtual bool VMLogin();

    virtual bool VMLogin(QByteArray msg);

    virtual bool VMSendMessage(QByteArray msg);

    virtual bool VMConnect();

    virtual bool ReConnect();

    virtual bool LoginOut();

    virtual bool InitState();

    virtual bool VMAuth(QByteArray msg);
};
