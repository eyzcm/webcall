#include "UIYL_ManRobotRuleSettingUrlWidget.h"

UIYL_ManRobotRuleSettingUrlWidget::UIYL_ManRobotRuleSettingUrlWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.m_btn_add, SIGNAL(clicked()), this, SLOT(R_Btn_Add_Clicked()));
    connect(ui.m_btn_del, SIGNAL(clicked()), this, SLOT(R_Btn_Del_Clicked()));
}

UIYL_ManRobotRuleSettingUrlWidget::~UIYL_ManRobotRuleSettingUrlWidget()
{
}

void UIYL_ManRobotRuleSettingUrlWidget::SetUrl(QString url)
{
    ui.m_edit_url->setText(url);
}

QString UIYL_ManRobotRuleSettingUrlWidget::GetUrl()
{
    return ui.m_edit_url->text();
}

bool UIYL_ManRobotRuleSettingUrlWidget::IsEmpty()
{
    bool empty = false;
    if (ui.m_edit_url->text().isEmpty())
    {
        empty = true;
    }
    return empty;
}

void UIYL_ManRobotRuleSettingUrlWidget::HideBtnAdd()
{
    ui.m_btn_add->hide();
}

void UIYL_ManRobotRuleSettingUrlWidget::ShowBtnAdd()
{
    ui.m_btn_add->show();
}

void UIYL_ManRobotRuleSettingUrlWidget::HideBtnDel()
{
    ui.m_btn_del->hide();
}

void UIYL_ManRobotRuleSettingUrlWidget::ShowBtnDel()
{
    ui.m_btn_del->show();
}

void UIYL_ManRobotRuleSettingUrlWidget::R_Btn_Add_Clicked()
{
    Q_EMIT S_AddUrl(this);
}

void UIYL_ManRobotRuleSettingUrlWidget::R_Btn_Del_Clicked()
{
    Q_EMIT S_DelUrl(this);
}
