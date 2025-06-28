#include "UISystemTipData.h"

UISystemTipData::UISystemTipData() : QObject()
{
    m_isEnable              = false;
    m_isBubbleTip           = false;
    m_isShowMainWndTip      = false;
    m_isFlashTip            = false;
    m_isToastTip            = false;
    m_voiceSize             = 2;
    m_isTopBusinessModule   = false;
    m_isSmallWindow         = false;
    m_visitorMsgNoReplyTime = 0;
    m_flashBgColor          = "#ff6445";
}

UISystemTipData::UISystemTipData(const UISystemTipData& other)
{
    this->m_isBubbleTip           = other.m_isBubbleTip;
    this->m_isEnable              = other.m_isEnable;
    this->m_isShowMainWndTip      = other.m_isShowMainWndTip;
    this->m_isFlashTip            = other.m_isFlashTip;
    this->m_type                  = other.m_type;
    this->m_voiceFileName         = other.m_voiceFileName;
    this->m_voiceType             = other.m_voiceType;
    this->m_voiceSize             = other.m_voiceSize;
    this->m_isToastTip            = other.m_isToastTip;
    this->m_isTopBusinessModule   = other.m_isTopBusinessModule;
    this->m_isSmallWindow         = other.m_isSmallWindow;
    this->m_visitorMsgNoReplyTime = other.m_visitorMsgNoReplyTime;
    this->m_flashBgColor          = other.m_flashBgColor;
}

UISystemTipData::UISystemTipData(UISystemTipData* other)
{
    this->m_isBubbleTip           = other->m_isBubbleTip;
    this->m_isEnable              = other->m_isEnable;
    this->m_isShowMainWndTip      = other->m_isShowMainWndTip;
    this->m_isFlashTip            = other->m_isFlashTip;
    this->m_type                  = other->m_type;
    this->m_voiceFileName         = other->m_voiceFileName;
    this->m_voiceType             = other->m_voiceType;
    this->m_voiceSize             = other->m_voiceSize;
    this->m_isToastTip            = other->m_isToastTip;
    this->m_isTopBusinessModule   = other->m_isTopBusinessModule;
    this->m_isSmallWindow         = other->m_isSmallWindow;
    this->m_visitorMsgNoReplyTime = other->m_visitorMsgNoReplyTime;
    this->m_flashBgColor          = other->m_flashBgColor;
}

UISystemTipData::~UISystemTipData()
{
}

QString UISystemTipData::GetType() const
{
    return m_type;
}

void UISystemTipData::SetType(QString val)
{
    m_type = val;
}

void UISystemTipData::IsEnable(bool val)
{
    m_isEnable = val;
}

bool UISystemTipData::IsEnable() const
{
    return m_isEnable;
}

void UISystemTipData::IsBubbleTip(bool val)
{
    m_isBubbleTip = val;
}

bool UISystemTipData::IsBubbleTip() const
{
    return m_isBubbleTip;
}

void UISystemTipData::IsShowMainWndTip(bool val)
{
    m_isShowMainWndTip = val;
}

bool UISystemTipData::IsShowMainWndTip() const
{
    return m_isShowMainWndTip;
}

void UISystemTipData::IsToastTip(bool val)
{
    m_isToastTip = val;
}

bool UISystemTipData::IsToastTip() const
{
    return m_isToastTip;
}

QString UISystemTipData::GetVoiceType() const
{
    return m_voiceType;
}

void UISystemTipData::SetVoiceType(QString val)
{
    m_voiceType = val;
}

int UISystemTipData::VisitorMsgNoReplyTime() const
{
    return m_visitorMsgNoReplyTime;
}

void UISystemTipData::VisitorMsgNoReplyTime(int val)
{
    m_visitorMsgNoReplyTime = val;
}

QString UISystemTipData::GetVoiceFileName() const
{
    return m_voiceFileName;
}

void UISystemTipData::SetVoiceFileName(QString val)
{
    m_voiceFileName = val;
}

bool UISystemTipData::IsFlashTip() const
{
    return m_isFlashTip;
}

void UISystemTipData::IsFlashTip(bool val)
{
    m_isFlashTip = val;
}

bool UISystemTipData::IsEqual(UISystemTipData* objData)
{
    bool isRet = true;
    if (objData)
    {
        do
        {
            if (this->GetType() != objData->GetType())
            {
                isRet = false;
                break;
            }
            if (this->IsEnable() != objData->IsEnable())
            {
                isRet = false;
                break;
            }
            if (this->IsBubbleTip() != objData->IsBubbleTip())
            {
                isRet = false;
                break;
            }
            if (this->IsShowMainWndTip() != objData->IsShowMainWndTip())
            {
                isRet = false;
                break;
            }
            if (this->IsToastTip() != objData->IsToastTip())
            {
                isRet = false;
                break;
            }
            if (this->IsFlashTip() != objData->IsFlashTip())
            {
                isRet = false;
                break;
            }
            if (this->FlashBgColor() != objData->FlashBgColor())
            {
                isRet = false;
                break;
            }

            if (this->VoiceSize() != objData->VoiceSize())
            {
                isRet = false;
                break;
            }
            if (this->GetVoiceType() != objData->GetVoiceType())
            {
                isRet = false;
                break;
            }
            if (this->GetVoiceFileName() != objData->GetVoiceFileName())
            {
                isRet = false;
                break;
            }
            if (this->IsTopBusinessModule() != objData->IsTopBusinessModule())
            {
                isRet = false;
                break;
            }
            if (this->IsSmallWindow() != objData->IsSmallWindow())
            {
                isRet = false;
                break;
            }
            if (this->VisitorMsgNoReplyTime() != objData->VisitorMsgNoReplyTime())
            {
                isRet = false;
                break;
            }
        } while (false);
    }
    else
    {
        isRet = false;
    }
    return isRet;
}

bool UISystemTipData::IsMerchantsEditionEqual(UISystemTipData* objData)
{
    bool isRet = true;
    if (objData)
    {
        do
        {
            if (this->GetType() != objData->GetType())
            {
                isRet = false;
                break;
            }
            if (this->IsEnable() != objData->IsEnable())
            {
                isRet = false;
                break;
            }
            if (this->IsBubbleTip() != objData->IsBubbleTip())
            {
                isRet = false;
                break;
            }
            if (this->IsShowMainWndTip() != objData->IsShowMainWndTip())
            {
                isRet = false;
                break;
            }
            if (this->IsToastTip() != objData->IsToastTip())
            {
                isRet = false;
                break;
            }
            if (this->VoiceSize() != objData->VoiceSize())
            {
                isRet = false;
                break;
            }
            /*if (this->IsFlashTip() != objData->IsFlashTip())
            {
                    isRet = false;
                    break;
            }*/
            if (this->GetVoiceType() != objData->GetVoiceType())
            {
                isRet = false;
                break;
            }
            if (this->GetVoiceFileName() != objData->GetVoiceFileName())
            {
                isRet = false;
                break;
            }
            if (this->IsTopBusinessModule() != objData->IsTopBusinessModule())
            {
                isRet = false;
                break;
            }
            if (this->IsSmallWindow() != objData->IsSmallWindow())
            {
                isRet = false;
                break;
            }
            if (this->VisitorMsgNoReplyTime() != objData->VisitorMsgNoReplyTime())
            {
                isRet = false;
                break;
            }
        } while (false);
    }
    else
    {
        isRet = false;
    }
    return isRet;
}

bool UISystemTipData::IsCustomizeSound()
{
    if (GetVoiceFileName().indexOf("CustomSound", 0, Qt::CaseInsensitive) == 0)
        return true;
    return false;
}

int UISystemTipData::VoiceSize() const
{
    return m_voiceSize;
}

void UISystemTipData::VoiceSize(int val)
{
    m_voiceSize = val;
}

bool UISystemTipData::IsTopBusinessModule() const
{
    return m_isTopBusinessModule;
}

void UISystemTipData::IsTopBusinessModule(bool val)
{
    m_isTopBusinessModule = val;
}

bool UISystemTipData::IsSmallWindow() const
{
    return m_isSmallWindow;
}

void UISystemTipData::IsSmallWindow(bool val)
{
    m_isSmallWindow = val;
}

QString UISystemTipData::FlashBgColor() const
{
    return m_flashBgColor;
}

void UISystemTipData::FlashBgColor(QString val)
{
    m_flashBgColor = val;
}
