#pragma once
#include "../baseprocess/ITcpWork.h"
#include "ElVmState.h"
#include "utils/Plat_EnumDefine.h"

#include <QObject>

#include <protocol/tcpmessage.h>
class TcpHvImp;
class ElVmStatusProcess : public QObject, ElVmState
{
    Q_OBJECT
public:
    ElVmStatusProcess();
    ~ElVmStatusProcess();
    static ElVmStatusProcess *getInstance();

    bool       InitVM(QString address, quint16 port, QString type);
    TcpHvImp    *CurrentTcpVMImp();
    ElVmStatus getStatus();
    void       setStatus(ElVmStatus value);
    bool       CheckSender();
    bool       connectVMservice();
    bool       reconnectVMservice();
    void       closeVMservice();
    void       LoginOk();
    //代理功能区
    bool VMConnect();
    bool VMLogin();
    bool VMLogin(QByteArray msg);
    bool VMSendTcpMessage(QByteArray msg);
    bool ReConnect();
    bool LoginOut();
    bool InitState();

    ITcpWork *TcpWork()
    {
        return m_tcpwork;
    }
    virtual bool VMAuth(QByteArray msg);

signals:
    void S_connected();
    void S_NetWorkError(int, QString);
    void S_MessageParsingFailed();
    void S_ReceivedMessage(TcpMessagePtr msg);
    void S_LoginOk();

private slots:
    void R_connected();
    void R_disconnected();
    void R_NetWorkError(int, QString);
    void R_ReceivedMessage(TcpMessagePtr msg);
    void R_LoginOk();

private:
    int       m_isUpdate;
    int       AlloceTcpImp();
    ITcpWork *m_tcpwork;

    static ElVmStatusProcess *m_connectProcess;

    ElVmState     **m_states;
    ElVmState      *m_istate;
    TcpHvImp         *m_currvmTcpImp;
    QList<TcpHvImp *> m_vmtcpimplist;
    int             m_init;
    int             m_maxTcpCount;
    int             m_currentTcp;
    ElVmStatus      m_currStatus;
    QString         m_vmaddress;
    QString         m_vmtype;
    quint16         m_vmport;
    int             m_loginCount;
};
