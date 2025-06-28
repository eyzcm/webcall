#include "UIEmotion.h"

UIEmotion::UIEmotion(void)
{
}

UIEmotion::UIEmotion(const UIEmotion& info)
{
    this->Key(info.Key());
    this->IconName(info.IconName());
    this->Tip(info.Tip());
}

UIEmotion::UIEmotion(UIEmotion* pinfo)
{
    if (pinfo)
    {
        this->Key(pinfo->Key());
        this->IconName(pinfo->IconName());
        this->Tip(pinfo->Tip());
    }
}

UIEmotion::~UIEmotion(void)
{
}

void UIEmotion::Tip(QString val)
{
    m_tip = val;
}

QString UIEmotion::Tip() const
{
    return m_tip;
}

void UIEmotion::Key(QString val)
{
    m_key = val;
}

QString UIEmotion::Key() const
{
    return m_key;
}

void UIEmotion::IconName(QString val)
{
    m_iconName = val;
}

QString UIEmotion::IconName() const
{
    return m_iconName;
}
