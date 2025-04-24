#ifndef UIYL_ManRobotRuleSettingWidget_H
#define UIYL_ManRobotRuleSettingWidget_H

#include "utils/EnumDefine.h"
#include "UIYL_ManRobotRuleSettingUrlWidget.h"
#include "UIYL_ManRobotRuleSetting_PreviewEffect_Widget.h"
#include "ui_UIYL_ManRobotRuleSettingWidget.h"
#include "data/UIActionRobotInfo.h"
#include "data/UIConditionOneInfo.h"
#include "data/UIConditionThreeInfo.h"
#include "data/UIConditionTwoInfo.h"
#include "data/UIManRobotInfo.h"
#include <QEvent>
#include <QTimer>
#include <QWidget>

class UIYL_ManRobotRuleSettingWidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotRuleSettingWidget(QWidget *parent = 0);
    ~UIYL_ManRobotRuleSettingWidget();

public:
    void SetWidgetType(int type);    // 0：新增，1：修改
    void SetBrokenNet(int isBroken); //设置网络状态

    void InitUI(UIManRobotInfo *info);
    void SetConditionOne(UIConditionOneInfo *conditionOne);
    void SetConditionTwo(UIConditionTwoInfo *conditionTwo);
    void SetConditionThree(UIConditionThreeInfo *conditionThree);
    void SetAction(UIActionRobotInfo *action);

    void ClearUI();
    void ShowMask();

signals:
    void S_HideWndAndDelCenterWidget();
    void S_HideNewAddWidget();

private slots:
    void R_StateChanged_CBox_VisitUrl(int state);
    void R_StateChanged_CBox_SearchKey(int state);
    void R_StateChanged_CBox_VisitorNoSayTime(int state);
    void R_StateChanged_CBox_VisitorSendMsgCount(int state);

    void R_AddUrl(UIYL_ManRobotRuleSettingUrlWidget *widget);
    void R_DelUrl(UIYL_ManRobotRuleSettingUrlWidget *widget);

    void R_Btn_SelectColor_Clicked();
    void R_Btn_Modify_Clicked();
    void R_Btn_Save_Clicked();

    void R_Timeout_HidePreviewEffectWidget();
    void R_TextEdited(const QString &value);
    void R_RuleItemClicked(QString id);
    void R_DelManRobotRule(QString id);
    void R_LoginStateChanged(ELOGINSTATUS oldStatus, ELOGINSTATUS newStatus);

private:
    void AddUrl(QString url = "");
    void ClearUrl();

    bool IsCheckNormal();
    void SaveByModify();
    void SaveByAdd();

private:
    void ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::UIYL_ManRobotRuleSettingWidget ui;

    int     m_nWidgetType; // 0：新增，1：修改
    QString m_strID;
    QString m_strModifyName;
    int     m_IsNetBroken;

    QWidget                                     *m_pMask;
    QVector<UIYL_ManRobotRuleSettingUrlWidget *> m_vecUrl;

    QColor                                         m_colorDefault;
    QTimer                                        *m_pTimer_HidePreviewEffectWidget;
    UIYL_ManRobotRuleSetting_PreviewEffect_Widget *m_pWidget_PreviewEffect;
};

#endif // UIYL_ManRobotRuleSettingWidget_H
