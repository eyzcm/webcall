#pragma once

#include "utils/EnumDefine.h"
#include "ui_UI_Widget_AuxiliaryFun_Panenl.h"
#include <QWidget>

class UI_Widget_AuxiliaryFun_Panenl : public QWidget
{
    Q_OBJECT

public:
    UI_Widget_AuxiliaryFun_Panenl(QWidget *parent = 0);
    ~UI_Widget_AuxiliaryFun_Panenl();
    QPoint getNoticepos(); //获取公告按钮位置

private:
    void CreateChatOnline();
    void InitSkinMenu();
    void InitCustomerMenu();
    void CreateAction(QMenu *menu, const QString &iconUri, const QString &text, const QString &data, int flag = 0);
    void ShowSkinMenu();
    void OpenUrl(QString url);
    void ShowNoticeUI();

    QString GetUrl(QString tempUri);

public slots:
    void R_Btn_Clicked();
    void R_GetUnreadAnnouncementResult(QString code, QString reason, QString body, int count);
    void R_ReadOneNotice();
    void R_OnMenuClicked();
    void R_ShowCustomerMenu();
    void R_SetUnreadMsgNum(int c, QString title, QString content, QString time);

signals:
    void S_LockScreen_Clicked();
    void S_ResetScale_Clicked();
    void S_ChangeSkin_Clicked(int type);
    void S_Assistant_Clicked();
    void S_ShowNotice_Clicked();
    void S_SetMainWndTop(bool isTopWnd);
    void S_ShowAnnouncement_Details_Clicked(); //显示通告通知

private:
    Ui::UI_Widget_AuxiliaryFun_PanenlClass ui;

    int    m_nUnreadNotice;
    bool   m_topWnd;
    QMenu *m_skinMenu;
    QMenu *m_customerMenu;
};
