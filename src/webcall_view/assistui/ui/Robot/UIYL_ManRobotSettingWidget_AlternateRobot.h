#pragma once
#include "utils/EnumDefine.h"
#include "ui_UIYL_ManRobotSettingWidget_AlternateRobot.h"
#include <QWidget>

class UIYL_ManRobotSettingWidget_AlternateRobot : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotSettingWidget_AlternateRobot(QWidget* parent = nullptr);
    ~UIYL_ManRobotSettingWidget_AlternateRobot();

public:
    void ShowMask();
    void ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);

public slots:
    void R_RBtn_Clicked();
    void R_Btn_Save_Clicked();
    void R_AddOrUpdateReserveRobotRelation_Result(QString code, QString reason, QString body);
    void R_Timeout();

private:
    void resizeEvent(QResizeEvent* event);

private:
    Ui::UIYL_ManRobotSettingWidget_AlternateRobot ui;
    QWidget*                                      m_pMask;

    bool    m_bIsResult;
    QTimer* m_pTimer;
};
