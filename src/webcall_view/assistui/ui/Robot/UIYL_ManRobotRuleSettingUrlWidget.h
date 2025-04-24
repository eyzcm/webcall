#pragma once

#include "ui_UIYL_ManRobotRuleSettingUrlWidget.h"
#include <QWidget>

class UIYL_ManRobotRuleSettingUrlWidget : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotRuleSettingUrlWidget(QWidget *parent = 0);
    ~UIYL_ManRobotRuleSettingUrlWidget();

public:
    void    SetUrl(QString url);
    QString GetUrl();

    bool IsEmpty();

    void HideBtnAdd();
    void ShowBtnAdd();

    void HideBtnDel();
    void ShowBtnDel();

signals:
    void S_AddUrl(UIYL_ManRobotRuleSettingUrlWidget *widget);
    void S_DelUrl(UIYL_ManRobotRuleSettingUrlWidget *widget);

private slots:
    void R_Btn_Add_Clicked();
    void R_Btn_Del_Clicked();

private slots:

private:
    Ui::UIYL_ManRobotRuleSettingUrlWidget ui;
};
