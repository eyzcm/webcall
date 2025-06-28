#ifndef PLAT_TCPMANAGERINTERFACE_H
#define PLAT_TCPMANAGERINTERFACE_H

#include <QObject>


#include "plat_interface_global.h"
class TcpMessage;

class PLAT_INTERFACE_EXPORT Plat_TcpManagerInterface:public QObject
{

    Q_OBJECT

public:
    Plat_TcpManagerInterface();
    ~Plat_TcpManagerInterface(){};


    virtual void ProcessAllNoEndTcpMessage()= 0;
    virtual void CreateTcpConnect(QString host, QString port)= 0;
    virtual void CreateTimer()= 0;
    virtual void CloseTimer()= 0;
    virtual void DisconnectTcp()= 0;

    virtual void ErrorLogin(QString msg)= 0;

    virtual TcpMessage* CreateNewResquest(){return nullptr;};

    virtual void    SendTcpMessage(TcpMessage* msg)= 0;
    virtual void    LoginOk()= 0;
    virtual int     GetErrorCode()= 0;
    virtual QString GetErrorMsg()= 0;


signals:

    void S_connected();
    void S_NetWorkError(int, QString);
    void S_MessageParsingFailed();
    void S_ReceivedMessage(QSharedPointer<TcpMessage> msg);

    void S_SendTcpMessage(QByteArray msg);
    void S_UpdataeRecordId(QString chatId, QString callbackId, QString recordId);


};

#endif // PLAT_TCPMANAGERINTERFACE_H
