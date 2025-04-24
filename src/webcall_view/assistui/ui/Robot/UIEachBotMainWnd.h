#ifndef UIEachBotMainWnd_H
#define UIEachBotMainWnd_H

#include "ui_UIEachBotMainWnd.h"
#include <QWidget>

class UIEachBotMainWnd : public QWidget
{
    Q_OBJECT

public:
    UIEachBotMainWnd(QWidget* parent = 0);
    ~UIEachBotMainWnd();

private:
    void InitSwitchButton(SwitchButton* btn, bool isCheck);

private slots:
    void R_OnRobotSettingBtnClick();
    void R_OnManRobotSettingBtnClick();
    void R_Btn_KeyPrompt_Clicked();
    void R_Btn_CountPrompt_Clicked();
    void R_Btn_AlternateRobot_Clicked();

    void R_ShowToastMsg_AISettingCenter(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);

signals:
    void S_Closed();
    void S_HideWndAndDelCenterWidget();
private:
    Ui::UIEachBotMainWnd ui;
};

#endif // UIEachBotMainWnd_H
