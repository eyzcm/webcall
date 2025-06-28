#pragma once
#include "utils/Plat_EnumDefine.h"
#include "platdata_global.h"
#include <QDateTime>
#include <QObject>


class PLATDTA_EXPORT UIHistoryInfo : public QObject
{
    Q_OBJECT
public:
    UIHistoryInfo(void);
    ~UIHistoryInfo(void);
    UIHistoryInfo(const UIHistoryInfo& info);
    UIHistoryInfo(UIHistoryInfo* info);

public:
    QString From() const;
    void    From(QString val);

    QString ChatId() const;
    void    ChatId(QString val);

    EMESSAGETYPE Type() const;
    void         Type(EMESSAGETYPE val);

    QString Body() const;
    void    Body(QString val);

    QDateTime Time() const;
    void      Time(QDateTime val);

    QString RecordId() const;
    void    RecordId(QString val);

private:
    QString      m_from;
    QString      m_chatId;
    EMESSAGETYPE m_type;
    QString      m_body;
    QDateTime    m_time;

    QString m_recordId;
};
