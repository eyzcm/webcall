#ifndef UIYL_ManRobotSettingWidget_H
#define UIYL_ManRobotSettingWidget_H

#include "utils/EnumDefine.h"
#include "UIYL_ManRobotRuleSearchResultWidget.h"
#include "ui/common/baseMainWnd.h"
#include "ui_UIYL_ManRobotSettingWidget.h"
#include <QWidget>
#include <ui/baseWnd.h>

class UIYL_ManRobotSettingWidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotSettingWidget(QWidget *parent = 0);
    ~UIYL_ManRobotSettingWidget();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *e);

signals:
    void S_ShowToastMsg_AISettingCenter(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);

private slots:
    void R_OnNewRuleBtnClicked();
    void R_OnSaveRuleListBtnClicked();
    void R_OnManRobotRuleListSearchLineTextChanged(const QString &text);
    void R_OnClickSearchItem(QString id);
    void R_DragItemChanged();
    void R_HideNewAddWidget();
	//void R_Init();

private:
    void ShowSearchWnd(QString text);
    void HideWnd();

private:
    Ui::UIYL_ManRobotSettingWidget       ui;
    BaseWnd                         *m_pAddNewRule;
    UIYL_ManRobotRuleSearchResultWidget *m_manRobotRuleSearchResultWidget;
	
};

#endif // UIYL_ManRobotSettingWidget_H
