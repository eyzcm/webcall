#pragma once

#include "utils/EnumDefine.h"
#include "UIYL_ManRobotRule_KeyWord_SearchResultWidget.h"
#include "ui/common/baseMainWnd.h"
#include "ui_UIYL_ManRobotSettingWidget_KeyPrompt.h"
#include <QWidget>
#include <ui/baseWnd.h>

class UIYL_ManRobotSettingWidget_KeyPrompt : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotSettingWidget_KeyPrompt(QWidget *parent = 0);
    ~UIYL_ManRobotSettingWidget_KeyPrompt();

signals:
    void S_ShowToastMsg_AISettingCenter(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);

private slots:
    void R_OnNewRuleBtnClicked();
    void R_OnSaveRuleListBtnClicked();
    void R_OnManRobotRuleListSearchLineTextChanged(const QString &text);
    void R_OnClickSearchItem(QString id);
    void R_DragItemChanged();
    void R_HideNewAddWidget();
    void R_ListViewHasData(bool hasData);
	//void R_Init();

private:
    void ShowSearchWnd(QString text);
    void HideWnd();

    virtual bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::UIYL_ManRobotSettingWidget_KeyPrompt      ui;
    BaseWnd                                  *m_pAddNewRule;
    UIYL_ManRobotRule_KeyWord_SearchResultWidget *m_manRobotRuleSearchResultWidget;

};
