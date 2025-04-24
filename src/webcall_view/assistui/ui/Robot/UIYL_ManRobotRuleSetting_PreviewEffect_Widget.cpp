#include "UIYL_ManRobotRuleSetting_PreviewEffect_Widget.h"

UIYL_ManRobotRuleSetting_PreviewEffect_Widget::UIYL_ManRobotRuleSetting_PreviewEffect_Widget(QWidget *parent) : QWidget(parent), m_nWidgetType(1)
{
    ui.setupUi(this);
}

UIYL_ManRobotRuleSetting_PreviewEffect_Widget::~UIYL_ManRobotRuleSetting_PreviewEffect_Widget()
{
}

void UIYL_ManRobotRuleSetting_PreviewEffect_Widget::SetWidgetType(int type)
{
    m_nWidgetType = type;

    QString style = "";
    if (type == 1)
    {
        this->setFixedSize(247, 334);
        ui.verticalLayout_2->setContentsMargins(15, 15, 16, 22);
        style = "QWidget#widget_bg{border-image: url(:/YL/Resources/YL/Robot/icon_previewEffect_bg.png);}";
    }
    else
    {
        this->setFixedSize(253, 328);
        ui.verticalLayout_2->setContentsMargins(15, 15, 22, 16);
        style = "QWidget#widget_bg{border-image: url(:/YL/Resources/YL/Robot/icon_previewEffect_bg_right.png);}";
    }
    ui.widget_bg->setStyleSheet(style);
}

void UIYL_ManRobotRuleSetting_PreviewEffect_Widget::SetChatItemColor(QColor color)
{
    QString strColor = QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()) + ", " + QString::number(color.alpha());
    QString strStyle = "background-color: rgba(" + strColor + ");";
    ui.m_widget_2_mid->setStyleSheet(strStyle);
}

void UIYL_ManRobotRuleSetting_PreviewEffect_Widget::SetChatItemColor(QString color)
{
    QString strStyle = "background-color: " + color + ";";
    ui.m_widget_2_mid->setStyleSheet(strStyle);
}

void UIYL_ManRobotRuleSetting_PreviewEffect_Widget::ResetChatItemColor()
{
    QString strStyle = "background-color: #FFEDED;";
    ui.m_widget_2_mid->setStyleSheet(strStyle);
}
