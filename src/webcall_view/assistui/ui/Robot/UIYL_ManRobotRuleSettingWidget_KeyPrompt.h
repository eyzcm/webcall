#pragma once

#include "utils/EnumDefine.h"
#include "UIYL_ManRobotRuleSetting_PreviewEffect_Widget.h"
#include "ui_UIYL_ManRobotRuleSettingWidget_KeyPrompt.h"
#include "data/UIUserRuleInfo.h"
#include <QEvent>
#include <QTimer>
#include <QWidget>

class UIYL_ManRobotRuleSettingWidget_KeyPrompt : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotRuleSettingWidget_KeyPrompt(QWidget *parent = 0);
    ~UIYL_ManRobotRuleSettingWidget_KeyPrompt();

public:
    void SetWidgetType(int type);    // 0：新增，1：修改
    void SetBrokenNet(int isBroken); //设置网络状态

    void InitUI(UIUserRuleInfo *info);
    void ClearUI();
    void ShowMask();

signals:
    void S_HideWndAndDelCenterWidget();
    void S_HideNewAddWidget();

public slots:
    void R_Btn_SelectColor_Clicked();
    void R_Btn_Modify_Clicked();
    void R_Btn_Save_Clicked();

    void R_Timeout_HidePreviewEffectWidget();
    void R_TextEdited(const QString &value);
    void R_RuleItemClicked(QString id);
    void R_DelManRobotRule(QString id);
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);

private:
    bool IsCheckNormal();
    void SaveByModify();
    void SaveByAdd();

private:
    void ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::UIYL_ManRobotRuleSettingWidget_KeyPrompt ui;

    int     m_nWidgetType; // 0：新增，1：修改
    QString m_strID;
    QString m_strModifyName;
    bool    m_IsNetBroken;

    QWidget                                       *m_pMask;
    QColor                                         m_colorDefault;
    QTimer                                        *m_pTimer_HidePreviewEffectWidget;
    UIYL_ManRobotRuleSetting_PreviewEffect_Widget *m_pWidget_PreviewEffect;
};
