#pragma once
#include "data/CommInfo.h"
#include "utils/Plat_EnumDefine.h"


class  Im_Message : public CommInfo
{
public:
    Im_Message(void)
    {
    }
    ~Im_Message(void)
    {
    }

public:
    EMESSAGETYPE           GetMsgType() const;
    const CommInfo*        Exts();
    QMap<QString, QString> ExtsMap() const;

private:
    CommInfo m_exts;
};

//{
//	"colleagueMessage": true,
//		"viewUserId": null,
//		"from": "haina",
//		"type": "message",
//		"fromUserId": "haina",
//		"msg": "好的"
//}
