#pragma once
#include "ElVmState.h"

class ElVmStatusProcess;

class ElVmSuccessState : public ElVmState
{

    ElVmStatusProcess* m_process;

public:
    ElVmSuccessState(ElVmStatusProcess* process);
    ~ElVmSuccessState(void);

    virtual bool VMLogin(QByteArray msg);

    virtual bool VMSendMessage(QByteArray msg);

    virtual bool VMConnect();

    virtual bool ReConnect();

    virtual bool LoginOut();

    virtual bool InitState();

    virtual bool VMLogin();

    virtual bool VMAuth(QByteArray msg);
};
