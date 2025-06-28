#include "MUICardInfoData.h"
#include <QDateTime>
#include <QDebug>
#include <QStringList>

MUICardInfoData::MUICardInfoData(void)
{
    Init();
}

void MUICardInfoData::InitCardInfo(const UICardInfo& info)
{
    if (!info.Id().isEmpty())
    {
        m_cardInfoMap.insert("createTime", info.CreateTime());
        m_cardInfoMap.insert("createUserId", info.CreateUserId());
        m_cardInfoMap.insert("editTime", info.EditTime());
        m_cardInfoMap.insert("editUserId", info.EditUserId());

        m_cardInfoMap.insert("area", info.Area());
        m_cardInfoMap.insert("companyName", info.CompanyName());
        m_cardInfoMap.insert("email", info.Email());
        m_cardInfoMap.insert("name", info.Name());
        m_cardInfoMap.insert("note", info.Note());
        m_cardInfoMap.insert("qq", info.Qq());
        m_cardInfoMap.insert("sex", info.Sex());
        m_cardInfoMap.insert("tel", info.Tel());
        m_cardInfoMap.insert("msn", info.Msn());
        m_cardInfoMap.insert("mobile", info.Mobile());
        m_cardInfoMap.insert("phoneStatus", info.PhoneStatus());
        m_cardInfoMap.insert("ntag", info.Tag());

        m_cardInfoMap.insert("extColumn1", info.ExtColumn1());
        m_cardInfoMap.insert("extColumn2", info.ExtColumn2());
        m_cardInfoMap.insert("extColumn3", info.ExtColumn3());
        m_cardInfoMap.insert("extColumn4", info.ExtColumn4());
        m_cardInfoMap.insert("extColumn5", info.ExtColumn5());
        m_cardInfoMap.insert("extColumn6", info.ExtColumn6());
        m_cardInfoMap.insert("extColumn7", info.ExtColumn7());
        m_cardInfoMap.insert("extColumn8", info.ExtColumn8());
        m_cardInfoMap.insert("extColumn9", info.ExtColumn9());
        m_cardInfoMap.insert("extColumn10", info.ExtColumn10());
        m_cardInfoMap.insert("extColumn11", info.ExtColumn11());
        m_cardInfoMap.insert("extColumn12", info.ExtColumn12());
        m_cardInfoMap.insert("extColumn13", info.ExtColumn13());
        m_cardInfoMap.insert("extColumn14", info.ExtColumn14());
        m_cardInfoMap.insert("extColumn15", info.ExtColumn15());
        m_cardInfoMap.insert("ip", info.Ip());
        m_cardInfoMap.insert("ipv6", info.Ipv6());
    }
    VisitorStaticId(info.VisitorStaticId());
}

MUICardInfoData::~MUICardInfoData(void)
{
}

void MUICardInfoData::Init()
{

    m_cardInfoMap.insert("createTime", "");
    m_cardInfoMap.insert("createUserId", "");
    m_cardInfoMap.insert("editTime", "");
    m_cardInfoMap.insert("editUserId", "");

    m_cardInfoMap.insert("area", "");
    m_cardInfoMap.insert("companyName", "");
    m_cardInfoMap.insert("email", "");
    m_cardInfoMap.insert("name", "");
    m_cardInfoMap.insert("note", "");
    m_cardInfoMap.insert("qq", "");
    m_cardInfoMap.insert("sex", "");
    m_cardInfoMap.insert("tel", "");
    m_cardInfoMap.insert("msn", "");
    m_cardInfoMap.insert("mobile", "");
    m_cardInfoMap.insert("phoneStatus", "");
    m_cardInfoMap.insert("ntag", "");

    m_cardInfoMap.insert("extColumn1", "");
    m_cardInfoMap.insert("extColumn2", "");
    m_cardInfoMap.insert("extColumn3", "");
    m_cardInfoMap.insert("extColumn4", "");
    m_cardInfoMap.insert("extColumn5", "");
    m_cardInfoMap.insert("extColumn6", "");
    m_cardInfoMap.insert("extColumn7", "");
    m_cardInfoMap.insert("extColumn8", "");
    m_cardInfoMap.insert("extColumn9", "");
    m_cardInfoMap.insert("extColumn10", "");
    m_cardInfoMap.insert("extColumn11", "");
    m_cardInfoMap.insert("extColumn12", "");
    m_cardInfoMap.insert("extColumn13", "");
    m_cardInfoMap.insert("extColumn14", "");
    m_cardInfoMap.insert("extColumn15", "");
    m_cardInfoMap.insert("ip", "");
    m_cardInfoMap.insert("ipv6", "");
}

void MUICardInfoData::ChatId(QString val)
{
    m_chatId = val;
}

QString MUICardInfoData::ChatId() const
{
    return m_chatId;
}

void MUICardInfoData::Vid(QString val)
{
    m_vid = val;
}

QString MUICardInfoData::Vid() const
{
    return m_vid;
}

void MUICardInfoData::VisitorStaticId(QString val)
{
    m_visitorStaticId = val;
}

QString MUICardInfoData::VisitorStaticId() const
{
    return m_visitorStaticId;
}

QString MUICardInfoData::GetUICardValueByKey(QString strKey) const
{
    QString ret = "";
    if (m_cardInfoMap.find(strKey) != m_cardInfoMap.end())
    {
        ret = m_cardInfoMap[strKey];
    }
    return ret;
}

void MUICardInfoData::SetUICardValueByKey(QString strKey, QString strVal)
{
    if (m_cardInfoMap.find(strKey) != m_cardInfoMap.end())
    {
        m_cardInfoMap.insert(strKey, strVal);
    }
}

const QMap<QString, QString> MUICardInfoData::GetCardInfo()
{
    return m_cardInfoMap;
}

void MUICardInfoData::SetCardInfo(QMap<QString, QString> cardInfoMap)
{
    m_cardInfoMap = cardInfoMap;
}

void MUICardInfoData::SetCardByString(const QString& content)
{
    QStringList itemList = content.split(",");
    Q_FOREACH (QString item, itemList)
    {
        if (item.isEmpty())
        {
            break;
        }
        QStringList valItemList = item.split("=");
        if (valItemList.count() == 2)
        {
            QString strVal = valItemList[1];
            if (strVal == "$LOCALTIME")
            {
                strVal = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            }
            SetUICardValueByKey(valItemList[0], strVal);
        }
    }
}
