#include "EventCustomCmd.h"
#include "utils/jsonhelper.h"

EventCustomCmd::EventCustomCmd(QString chatId) : TcpCmd()
{
    m_chatId = chatId;
}

EventCustomCmd::~EventCustomCmd()
{
}

void EventCustomCmd::MakeCmd()
{
    Uri(QString("/Message?chatId=%1&type=eventCustom").arg(m_chatId));
    this->IsJsontype(true);
    this->AddMap("subType", QString("%1").arg(m_subType));
    Json::Value val;
    for (auto iter = m_map.begin(); iter != m_map.end(); iter++)
    {

        if (iter.key() == "subType")
            val[iter.key().toStdString()] = iter.value().toStdString();

        val[(iter.key()).toStdString()] = iter.value().toStdString();
    }

    QString json = QString::fromStdString(val.toStyledString());

    val["extBodyJson"] = json.toStdString();
    this->Body(QString::fromStdString(val.toStyledString()));
}

void EventCustomCmd::AddMap(QString key, QString val)
{
    if (m_map.find(key) == m_map.end())
        m_map.insert(key, val);
}
