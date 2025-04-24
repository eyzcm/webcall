#pragma once

#include "ui_UIYL_ManRobotRuleSetting_PreviewEffect_Widget.h"
#include <QColor>
#include <QWidget>

class UIYL_ManRobotRuleSetting_PreviewEffect_Widget : public QWidget
{
    Q_OBJECT

public:
    UIYL_ManRobotRuleSetting_PreviewEffect_Widget(QWidget *parent = 0);
    ~UIYL_ManRobotRuleSetting_PreviewEffect_Widget();

public:
    void SetWidgetType(int type); // 1:接通时提醒  2:关键词提醒  3:索联次数提醒
    void SetChatItemColor(QColor color);
    void SetChatItemColor(QString color);
    void ResetChatItemColor();

private:
    Ui::UIYL_ManRobotRuleSetting_PreviewEffect_Widget ui;
    int                                               m_nWidgetType;
};
