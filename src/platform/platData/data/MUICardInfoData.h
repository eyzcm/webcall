#pragma once
#include "ui/data/MBaseData.h"
#include "data/UICardInfo.h"
#include <QMap>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT MUICardInfoData : public MBaseData
{
    Q_OBJECT
public:
    MUICardInfoData(void);
    ~MUICardInfoData(void);
    void InitCardInfo(const UICardInfo& info);
    void SetCardByString(const QString& content);

private:
    QString                m_chatId;
    QString                m_vid;
    QString                m_visitorStaticId;
    QMap<QString, QString> m_cardInfoMap;

private:
    void Init();

public:
    QString VisitorStaticId() const;
    void    VisitorStaticId(QString val);

    QString Vid() const;
    void    Vid(QString val);

    QString ChatId() const;
    void    ChatId(QString val);

    QString GetUICardValueByKey(QString strKey) const;
    void    SetUICardValueByKey(QString strKey, QString strVal);

    // CZC：获取名片数据
    const QMap<QString, QString> GetCardInfo();
    void                         SetCardInfo(QMap<QString, QString> cardInfoMap);
};
