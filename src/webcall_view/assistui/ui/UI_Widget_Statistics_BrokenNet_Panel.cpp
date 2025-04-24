#include "UI_Widget_Statistics_Panel.h"

UI_Widget_Statistics_BrokenNet_Panel::UI_Widget_Statistics_BrokenNet_Panel(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
}

UI_Widget_Statistics_BrokenNet_Panel::~UI_Widget_Statistics_BrokenNet_Panel() {}

void UI_Widget_Statistics_BrokenNet_Panel::SetErrorFlag(int flag)
{
    if (flag == 0)
    {
        ui.m_errorinfo->setText(tr("网络连接已断开！"));
    }
    else
    {
        ui.m_errorinfo->setText(tr("可能是token过期需要重新登录！"));
    }
}
