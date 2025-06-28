#include "UIHistoryInfo.h"

UIHistoryInfo::UIHistoryInfo(void)
{
    m_type = E_MSGTYPE_MESSAGE;
}

UIHistoryInfo::UIHistoryInfo(const UIHistoryInfo& info)
{
    this->Body(info.Body());
    this->ChatId(info.ChatId());
    this->From(info.From());
    this->Time(info.Time());
    this->Type(info.Type());
    this->RecordId(info.RecordId());
}
UIHistoryInfo::UIHistoryInfo(UIHistoryInfo* info)
{
    this->Body(info->Body());
    this->ChatId(info->ChatId());
    this->From(info->From());
    this->Time(info->Time());
    this->Type(info->Type());
    this->RecordId(info->RecordId());
}

UIHistoryInfo::~UIHistoryInfo(void)
{
}

void UIHistoryInfo::From(QString val)
{
    m_from = val;
}

QString UIHistoryInfo::From() const
{
    return m_from;
}

void UIHistoryInfo::ChatId(QString val)
{
    m_chatId = val;
}

QString UIHistoryInfo::ChatId() const
{
    return m_chatId;
}

void UIHistoryInfo::Type(EMESSAGETYPE val)
{
    m_type = val;
}

EMESSAGETYPE UIHistoryInfo::Type() const
{
    return m_type;
}

void UIHistoryInfo::Body(QString val)
{
    m_body = val;
}

QString UIHistoryInfo::Body() const
{
    return m_body;
}

void UIHistoryInfo::Time(QDateTime val)
{
    m_time = val;
}

QDateTime UIHistoryInfo::Time() const
{
    return m_time;
}

QString UIHistoryInfo::RecordId() const
{
    return m_recordId;
}

void UIHistoryInfo::RecordId(QString val)
{
    m_recordId = val;
}
