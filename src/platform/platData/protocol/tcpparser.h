#ifndef TCPPARSER_H
#define TCPPARSER_H

#include "utils/DefineHelper.h"

#include "tcpmessage.h"
#include <QObject>
#include <QStringList>
#include "platdata_global.h"

class PLATDTA_EXPORT  TcpParser : public QObject
{
    Q_OBJECT

public:
    TcpParser();
    ~TcpParser();

public:
    void Parse(QByteArray recvBuff);

public:
    Property_QString(ProtocolTag, protocolTag);

private:
    bool TryToParse();
    //解析第一行
    bool ParseFirstLine(QString val, TcpMessage* msg);
    //解析头
    bool ParseHeaderLine(QString val, TcpMessage* msg);
signals:
    void S_MessageParsingFailed();
    void S_ReceivedMessage(TcpMessagePtr msg);

private:
    QStringList  s_validMethods;
    QByteArray   m_protocolBuff;
    const qint16 m_MaxMessageLength;
};

#endif // TCPPARSER_H
