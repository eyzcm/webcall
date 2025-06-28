#include "Im_Message.h"
#include "utils/utilshelper.h"

EMESSAGETYPE Im_Message::GetMsgType() const
{
    return UtilsHelper::GetMessageTypeByString(GetValueByKey("type"));
}

const CommInfo* Im_Message::Exts()
{
    return &m_exts;
}

QMap<QString, QString> Im_Message::ExtsMap() const
{
    return m_exts.InfoMap();
}
