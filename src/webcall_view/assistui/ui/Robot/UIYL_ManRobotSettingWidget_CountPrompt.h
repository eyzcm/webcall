#pragma once
#include <QColor>
#include <QColorDialog>
#include <QListView>
#include <QWidget>

#include "utils/EnumDefine.h"
#include "UIObserverImp.h"
#include "UIYL_ManRobotRuleSetting_PreviewEffect_Widget.h"
#include "ui_UIYL_ManRobotSettingWidget_CountPrompt.h"
#include "data/UIUserRuleInfo.h"

class UIYL_ManRobotSettingWidget_CountPrompt : public QWidget, public IUIObserverImp
{
    Q_OBJECT

public:
    UIYL_ManRobotSettingWidget_CountPrompt(QWidget *parent = 0);
    ~UIYL_ManRobotSettingWidget_CountPrompt();

public:
    virtual void Update(QString type, QObject *obj);
    virtual void RemoveUpdate(QString type, QString body);

public:
    void SetBrokenNet(int isBroken); //设置网络状态

    void InitUI(UIUserRuleInfo *info);
    void ClearUI();
    void ShowMask();

public slots:
    void R_Btn_SelectColor_Clicked();
    void R_Btn_Modify_Clicked();
    void R_Btn_Save_Clicked();
    void R_Timeout_HidePreviewEffectWidget();
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);
	//void R_Init();

private:
    bool IsCheckNormal();
    void SaveByModify();

private:
    void ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::UIYL_ManRobotSettingWidget_CountPrompt ui;

    QString m_strID;
    QString m_strRuleName;
    bool    m_IsNetBroken;

    QWidget                                       *m_pMask;
    QColor                                         m_colorDefault;
    QTimer                                        *m_pTimer_HidePreviewEffectWidget;
    UIYL_ManRobotRuleSetting_PreviewEffect_Widget *m_pWidget_PreviewEffect;

};
