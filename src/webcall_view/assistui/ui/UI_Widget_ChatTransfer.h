#pragma once

#include "utils/EnumDefine.h"
#include "data/YLData/YL_MainTreeLeafNodeItemData.h"
#include "ui_UI_Widget_ChatTransfer.h"
#include <QAbstractTextDocumentLayout>
#include <QEvent>
#include <QTimer>
#include <QWidget>

class UI_Widget_ChatTransfer : public QWidget
{
    Q_OBJECT

public:
    UI_Widget_ChatTransfer(QWidget *parent = 0);
    ~UI_Widget_ChatTransfer();

public:
    void InitData(YL_MainTreeLeafNodeItemData *nodeLeaf);

    void SetIcon_EngineType();
    void SetVNameAndVID(QString vid);
    void SetIpInfo(QString chatID, QString vid);
    void SetVisitorTime(QString vid);
    void SetCardTagInfo(QString userid);
    void SetPreCustomerText(QString &chatID, EYLUI_NODETYPE noteType);

    QString FormatString(QLabel *label, QString &str);
    QString GetSearchEngineIcon_Original();

signals:
    void S_Closed();
    void S_Btn_Refuse_Clicked(YL_MainTreeLeafNodeItemData *nodeLeaf);
    void S_Btn_Accept_Clicked(YL_MainTreeLeafNodeItemData *nodeLeaf);

private slots:
    void R_TimeOut();
    void R_Btn_Refuse_Clicked();
    void R_Btn_Accept_Clicked();

private:
    Ui::UI_Widget_ChatTransfer ui;

    int                          m_nTiming;
    QTimer                      *m_pTimer;
    YL_MainTreeLeafNodeItemData *m_pChat;

    QString        m_staticID;
    QString        m_visitorID;
    QString        m_chatID;
    QString        m_searchEngineId;
    QString        m_strPreCustomerID;
    QString        m_extIp;
    QString        m_strSourceLeafNodeName;
    EYLUI_NODETYPE m_nodeType;
    ECHATITEMSTATE m_chatState;
};
