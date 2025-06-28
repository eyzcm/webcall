#ifndef TCPMESSAGE_H
#define TCPMESSAGE_H

#include <QDataStream>
#include <QMap>
#include <QObject>

#include "utils/DefineHelper.h"
#include "platdata_global.h"
#include <qMetaType>
#include <qsharedpointer.h>
class TcpMessage;
typedef QSharedPointer<TcpMessage> TcpMessagePtr;

class PLATDTA_EXPORT TcpMessage : public QObject
{
    Q_OBJECT

public:
    
	TcpMessage(TcpMessage * tcpmsg  = NULL);
    ~TcpMessage();

signals:
    void S_SendMessageFinished(TcpMessagePtr resp);

public:
    void                   AddHeader(QString key, QString val);
    QString                GetHeaderByName(QString key);
    QMap<QString, QString> GetHeaderList();

    QString                GetPramsByName(QString key);
    QMap<QString, QString> GetPramsList();

    bool HasResponse() const
    {
        return m_hasResponse;
    }
    void HasResponse(bool val)
    {
        m_hasResponse = val;
    }

    bool IsJsontype() const
    {
        return m_jsontype;
    }
    void IsJsontype(bool val)
    {
        m_jsontype = val;
    }

    void SendMessageResponse(TcpMessagePtr resp);

public:
    Property_QString(ProtocolTag, protocolTag);
    Property_QString(Body, body);
    Property_QString(Seq, seq);
    Property_Int(ContentLength, contentLength);

public:
    QString Uri() const;
    void    Uri(QString v);

public:
    virtual QByteArray ToString();

protected:
    QString m_uri;

private:
    QMap<QString, QString> m_mapHeader;

    QMap<QString, QString> m_mapPramsList;
    bool                   m_hasResponse;
    bool                   m_jsontype;
};






//#define DEFTCPMESSAGE(x,y)  TcpMessage * x = y
#define DEFTCPMESSAGE(x,y)  QSharedPointer<TcpMessage> x(new TcpMessage(y) );

Q_DECLARE_METATYPE(TcpMessagePtr);

#endif // TCPMESSAGE_H
