#include "tcpmessage.h"
#include <QStringList>
#include "service/util_service.h"


TcpMessage::~TcpMessage()
{
    m_mapHeader.clear();
}

TcpMessage::TcpMessage(TcpMessage * tcpmsg)
{
	if (tcpmsg)
	{
		ProtocolTag(tcpmsg->ProtocolTag());
		Body(tcpmsg->Body());
		HasResponse(tcpmsg->HasResponse());
		Seq(tcpmsg->Seq());
		ContentLength(tcpmsg->ContentLength());
		Uri(tcpmsg->Uri());
		IsJsontype(tcpmsg->IsJsontype());

		foreach(const QString str, tcpmsg->GetHeaderList().keys())
		{
			this->AddHeader(str, tcpmsg->GetHeaderList().value(str));
			
		}

		foreach(const QString str, tcpmsg->GetPramsList().keys())
		{
			m_mapPramsList.insert(str, tcpmsg->GetPramsList().value(str));

		}


	}
	else
	{
		ProtocolTag("R");
		m_body = "OK";
		m_hasResponse = false;
		m_mapHeader.clear();
		m_jsontype = false;
	}
	

}

void TcpMessage::AddHeader(QString key, QString val)
{
    m_mapHeader.insert(key.trimmed(), val);
}

QString TcpMessage::GetHeaderByName(QString key)
{

    if (m_mapHeader.find(key) == m_mapHeader.end())
    {
        return "";
    }
    return m_mapHeader[key];
}

QMap<QString, QString> TcpMessage::GetHeaderList()
{
    return m_mapHeader;
}

QByteArray TcpMessage::ToString()
{
    QByteArray ret             = "";
    QString    protocol        = "";
    QString    firstLineFormat = "R %3 %2 %1";

    int val = Body().toUtf8().length();
    ContentLength(val);
    QString prams = "";

    for (auto itor = m_mapPramsList.begin(); itor != m_mapPramsList.end(); itor++)
    {
        QString key    = itor.key();
        QString val    = itor.value();
        QString result = QString("%1=%2&").arg(key).arg(val);
        prams.append(result);
    }

    QString tempUri = Uri();
    if (!prams.isEmpty())
    {
        prams = prams.mid(0, prams.length() - 1);
        tempUri.append("?");
        tempUri.append(prams);
    }

    QString firstLine = firstLineFormat.arg(Seq()).arg(val).arg(tempUri);
    protocol.append(firstLine);
    protocol.append("\r\n");
    if (m_jsontype)
    {
        protocol.append("Content-Type:application/json");
        protocol.append("\r\n");
    }

    QString headerFormat = "%1:%2";
    for (auto begin = m_mapHeader.begin(); begin != m_mapHeader.end(); begin++)
    {
        protocol.append(headerFormat.arg(begin.key()).arg(begin.value()));
        protocol.append("\r\n");
    }

    protocol.append("\r\n");
    if (!Body().isEmpty())
    {
        protocol.append(Body());
    }
    else
    {
        // protocol.append("\r\n");
    }
    ret = protocol.toUtf8();

    return ret;
}

void TcpMessage::SendMessageResponse(TcpMessagePtr resp)
{
    Q_EMIT S_SendMessageFinished(resp);
}

QString TcpMessage::Uri() const
{
    return m_uri;
}

void TcpMessage::Uri(QString v)
{
    if (v.indexOf("?") != -1)
    {
        QStringList list = v.split("?");
        if (list.size() > 0)
        {
            m_uri = list[0];

            QString     prams     = list[1];
            QStringList pramsList = prams.split("&");
            Q_FOREACH (QString item, pramsList)
            {
                QStringList itemList = item.split("=");
                if (itemList.size() == 2)
                {
                    m_mapPramsList.insert(itemList[0], itemList[1]);
                }
            }
        }
    }
    else
    {
        m_uri = v;
    }
}

QString TcpMessage::GetPramsByName(QString key)
{
    QString ret = "";
    if (m_mapPramsList.find(key) != m_mapPramsList.end())
    {
        ret = m_mapPramsList[key];
    }
    return ret;
}

QMap<QString, QString> TcpMessage::GetPramsList()
{
    return m_mapPramsList;
}
