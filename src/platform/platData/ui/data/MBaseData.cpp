#include "MBaseData.h"

MBaseData::MBaseData(void)
{
}

MBaseData::MBaseData(const MBaseData& o)
{
}

MBaseData::~MBaseData(void)
{
}

void MBaseData::AddExtendData(QString key, QString val)
{
    m_extendInfo[key.toLower()] = val;
}

QString MBaseData::GetExtendData(QString key) const
{
    QString ret = "";
    if (m_extendInfo.find(key.toLower()) != m_extendInfo.end())
    {
        ret = m_extendInfo[key.toLower()];
    }
    return ret;
}
