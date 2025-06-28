#ifndef UISYSTEMTIPDATA_H
#define UISYSTEMTIPDATA_H

#include "platdata_global.h"
#include <QObject>

class PLATDTA_EXPORT UISystemTipData : public QObject
{
    Q_OBJECT

public:
    UISystemTipData();
    UISystemTipData(const UISystemTipData& other);
    UISystemTipData(UISystemTipData* other);
    ~UISystemTipData();

    QString GetType() const;
    void    SetType(QString val);

    bool IsEnable() const;
    void IsEnable(bool val);

    bool IsBubbleTip() const;
    void IsBubbleTip(bool val);

    bool IsShowMainWndTip() const;
    void IsShowMainWndTip(bool val);

    bool IsToastTip() const;
    void IsToastTip(bool val);

    bool IsFlashTip() const;
    void IsFlashTip(bool val);

    QString GetVoiceType() const;
    void    SetVoiceType(QString val);

    int  VoiceSize() const;
    void VoiceSize(int val);

    bool IsTopBusinessModule() const;
    void IsTopBusinessModule(bool val);

    bool IsSmallWindow() const;
    void IsSmallWindow(bool val);

    int  VisitorMsgNoReplyTime() const;
    void VisitorMsgNoReplyTime(int val);

    QString GetVoiceFileName() const;
    void    SetVoiceFileName(QString val);

    bool IsEqual(UISystemTipData* objData);

    bool IsMerchantsEditionEqual(UISystemTipData* objData);

    bool IsCustomizeSound();

    QString FlashBgColor() const;
    void    FlashBgColor(QString val);

private:
    QString m_type;
    bool    m_isEnable;
    bool    m_isBubbleTip;
    bool    m_isShowMainWndTip;
    bool    m_isToastTip;
    bool    m_isFlashTip;
    QString m_voiceType;
    QString m_voiceFileName;
    int     m_voiceSize;
    bool    m_isTopBusinessModule;
    bool    m_isSmallWindow;
    int     m_visitorMsgNoReplyTime;
    QString m_flashBgColor;
};

#endif // UISYSTEMTIPDATA_H
