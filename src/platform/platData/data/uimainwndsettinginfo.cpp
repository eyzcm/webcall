#include "uimainwndsettinginfo.h"

UIMainWndSettingInfo::UIMainWndSettingInfo(QObject *parent) : QObject(parent)
{

    m_enableKeyCommonWordSearch = 0;
    m_enableKeyChatChange = 0;
    m_enableKeyChatChangeLongestTime = 0;

    m_enableKeyCloseSelectedChat = 0;
    m_commonWordDisplayPos = 0;

    m_leftPanelDefaultWidth_VisitorChat = 0;
    m_enableKeyOpenCard = 0;

    m_rightPanelDefaultWidth = 0;
    m_enableKeyActiveMainWnd = 0;



}


int  UIMainWndSettingInfo::EnableKeyCommonWordSearch() const
{
    return m_enableKeyCommonWordSearch;
}
void UIMainWndSettingInfo::EnableKeyCommonWordSearch(int val)
{
    m_enableKeyCommonWordSearch = val;
}

QString UIMainWndSettingInfo::KeyCommonWordSearch() const
{
    return m_keyCommonWordSearch;
}
QString UIMainWndSettingInfo::KeyOpenCard() const
{

    return m_keyOpenCard;
}
void    UIMainWndSettingInfo::KeyOpenCard(QString val)
{
    m_keyOpenCard = val;
}
void    UIMainWndSettingInfo::KeyCommonWordSearch(QString val)
{
    m_keyCommonWordSearch = val;
}


int  UIMainWndSettingInfo::EnableKeyChatChange() const
{
    return m_enableKeyChatChange;
}
void UIMainWndSettingInfo::EnableKeyChatChange(int val)
{
    m_enableKeyChatChange =val;
}

int  UIMainWndSettingInfo::EnableKeyChatChangeLongestTime() const
{
    return m_enableKeyChatChangeLongestTime;
}
void UIMainWndSettingInfo::EnableKeyChatChangeLongestTime(int val)
{
    m_enableKeyChatChangeLongestTime = val;
}

int  UIMainWndSettingInfo::EnableKeyCloseSelectedChat() const
{
    return m_enableKeyCloseSelectedChat;
}
void UIMainWndSettingInfo::EnableKeyCloseSelectedChat(int val)
{
    m_enableKeyCloseSelectedChat = val;
}

QString UIMainWndSettingInfo::KeyCloseSelectedChat() const
{
    return m_keyCloseSelectedChat;
}
void    UIMainWndSettingInfo::KeyCloseSelectedChat(QString val)
{
    m_keyCloseSelectedChat = val;
}

int  UIMainWndSettingInfo::CommonWordDisplayPos() const
{
    return m_commonWordDisplayPos;
}
void UIMainWndSettingInfo::CommonWordDisplayPos(int val)
{
    m_commonWordDisplayPos = val;
}

// CZC：医疗版左侧面板（访客对话）默认展示宽度
void UIMainWndSettingInfo::LeftPanelDefaultWidth_VisitorChat(int val)
{
    m_leftPanelDefaultWidth_VisitorChat = val;
}
int  UIMainWndSettingInfo::LeftPanelDefaultWidth_VisitorChat() const
{
    return m_leftPanelDefaultWidth_VisitorChat;
}


int  UIMainWndSettingInfo::EnableKeyOpenCard() const
{
    return m_enableKeyOpenCard;
}
void UIMainWndSettingInfo::EnableKeyOpenCard(int val)
{
    m_enableKeyOpenCard = val;
}

QString UIMainWndSettingInfo::KeyChatChange() const
{
    return m_keyChatChange;
}
void    UIMainWndSettingInfo::KeyChatChange(QString val)
{
    m_keyChatChange = val;
}

// CZC：常用语名片面板默认展示宽度
void UIMainWndSettingInfo::RightPanelDefaultWidth(int val)
{
    m_rightPanelDefaultWidth = val;
}
int  UIMainWndSettingInfo::RightPanelDefaultWidth() const
{
    return m_rightPanelDefaultWidth;
}

int  UIMainWndSettingInfo::EnableKeyActiveMainWnd() const
{
    return m_enableKeyActiveMainWnd;
}
void UIMainWndSettingInfo::EnableKeyActiveMainWnd(int val)
{
    m_enableKeyActiveMainWnd = val;
}

QString UIMainWndSettingInfo::KeyActiveMainWnd() const
{
    return m_keyActiveMainWnd;
}
void    UIMainWndSettingInfo::KeyActiveMainWnd(QString val)
{
    m_keyActiveMainWnd = val;
}



