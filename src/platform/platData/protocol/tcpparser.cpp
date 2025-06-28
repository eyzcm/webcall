#include "utils/EL_Util_Time.h"
#include "service/util_service.h"

//#include "protocol_biz/protocol_agent.h"
#include "tcpparser.h"
#include <QStringList>
#include <QSharedPointer>

TcpParser::TcpParser() : QObject(), m_MaxMessageLength(256 * 256)
{
    m_protocolBuff = "";
    ProtocolTag("R");
}

TcpParser::~TcpParser()
{
}

void TcpParser::Parse(QByteArray recvBuff)
{
    m_protocolBuff.append(recvBuff);
    bool isTryParse = TryToParse();
    if (!isTryParse)
    {
        Q_ASSERT(false);
    }
}

bool TcpParser::TryToParse()
{
    do
    {
        if (!m_protocolBuff.startsWith("R "))
        {
            Q_EMIT S_MessageParsingFailed();
            return false;
        }

        QString val    = QString::fromUtf8(m_protocolBuff);
        int     nIndex = val.indexOf("\r\n\r\n");
        if (nIndex != -1)
        {
			

            TcpMessage* msg = new TcpMessage();
			

            QString     header     = val.left(nIndex);
            QStringList headerList = header.split("\r\n");
            for (int i = 0; i < headerList.count(); i++)
            {
				
                if (0 == i)
                {
                    // first line;
                    ParseFirstLine(headerList[i], msg);
                }
                else
                {
                    //解析头
                    ParseHeaderLine(headerList[i], msg);
                }
            }

            if (m_protocolBuff.size() < msg->ContentLength() + 4 + nIndex)
            {
                //msg->deleteLater();
				delete msg;
                msg = nullptr;
                break;
            }

			//zcm 修复body取值长度问题 
			QString body = QString::fromUtf8(m_protocolBuff.mid(nIndex + 4, msg->ContentLength()));//val.mid(nIndex + 4, msg->ContentLength());
            //QString msgLog = QString::fromUtf8(m_protocolBuff.left(nIndex + 4 + msg->ContentLength()));

			QByteArray tempBuff = m_protocolBuff.mid(nIndex + 4 + msg->ContentLength(), -1);
			
			
            msg->Body(body);
			
			
			m_protocolBuff = tempBuff;
            {

                ET(QString("Q_EMIT S_ReceivedMessage( %1 );").arg((unsigned long)msg));

				DEFTCPMESSAGE(tmsg, msg);
                Q_EMIT S_ReceivedMessage(tmsg);
            }
			delete msg;
			msg = nullptr;

        }
        else
        {
            return true;
        }
    } while (m_protocolBuff.startsWith("R "));
    return true;
}

bool TcpParser::ParseFirstLine(QString val, TcpMessage* msg)
{
    // R /Auth 33 seq_11
    if (!val.startsWith("R "))
    {
        return false;
    }
    QStringList list = val.split(" ");
    if (list.count() < 4)
    {
        return false;
    }
    msg->ProtocolTag(list[0]);
    msg->Uri(list[1]);
    msg->ContentLength(list[2].toInt());
    msg->Seq(list[3]);
    return true;
}

bool TcpParser::ParseHeaderLine(QString val, TcpMessage* msg)
{
    // Auth:error
    if (val.isEmpty())
    {
        return true;
    }
    int nIndex = val.indexOf(":");
    if (nIndex == -1)
    {
        Q_ASSERT(false);
        return true;
    }
    QString key   = val.left(nIndex);
    QString value = val.mid(nIndex + 1, -1);
    msg->AddHeader(key, value);
    return true;
}
