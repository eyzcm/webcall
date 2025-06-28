#ifndef UIMAINWNDSETTINGINFO_H
#define UIMAINWNDSETTINGINFO_H

#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT UIMainWndSettingInfo : public QObject
{
    Q_OBJECT
public:
    explicit UIMainWndSettingInfo(QObject *parent = nullptr);

    int  EnableKeyCommonWordSearch() const;
    void EnableKeyCommonWordSearch(int val);

    QString KeyCommonWordSearch() const;
    QString KeyOpenCard() const;
    void    KeyOpenCard(QString val);
    void    KeyCommonWordSearch(QString val);


    int  EnableKeyChatChange() const;
    void EnableKeyChatChange(int val);

    int  EnableKeyChatChangeLongestTime() const;
    void EnableKeyChatChangeLongestTime(int val);

    int  EnableKeyCloseSelectedChat() const;
    void EnableKeyCloseSelectedChat(int val);

    QString KeyCloseSelectedChat() const;
    void    KeyCloseSelectedChat(QString val);

    int  CommonWordDisplayPos() const;
    void CommonWordDisplayPos(int val);

    // CZC：医疗版左侧面板（访客对话）默认展示宽度
    void LeftPanelDefaultWidth_VisitorChat(int val);
    int  LeftPanelDefaultWidth_VisitorChat() const;


    int  EnableKeyOpenCard() const;
    void EnableKeyOpenCard(int val);

    QString KeyChatChange() const;
    void    KeyChatChange(QString val);

    // CZC：常用语名片面板默认展示宽度
    void RightPanelDefaultWidth(int val);
    int  RightPanelDefaultWidth() const;

    int  EnableKeyActiveMainWnd() const;
    void EnableKeyActiveMainWnd(int val);

    QString KeyActiveMainWnd() const;
    void    KeyActiveMainWnd(QString val);



signals:

private:

    int  m_enableKeyCommonWordSearch;

    QString m_keyCommonWordSearch;
    QString m_keyOpenCard;


    int  m_enableKeyChatChange;

    int  m_enableKeyChatChangeLongestTime;

    int  m_enableKeyCloseSelectedChat;

    QString m_keyCloseSelectedChat;

    int  m_commonWordDisplayPos;

    int  m_leftPanelDefaultWidth_VisitorChat;
    int  m_enableKeyOpenCard;

    QString m_keyChatChange;

    int  m_rightPanelDefaultWidth;
    int  m_enableKeyActiveMainWnd;
    QString m_keyActiveMainWnd;


};

#endif // UIMAINWNDSETTINGINFO_H
