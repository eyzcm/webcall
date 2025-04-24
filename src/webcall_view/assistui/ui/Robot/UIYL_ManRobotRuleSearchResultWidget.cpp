#include "UIYL_ManRobotRuleSearchResultWidget.h"
#include "model/UIManRobotMatchRuleTipModel.h"

UIYL_ManRobotRuleSearchResultWidget::UIYL_ManRobotRuleSearchResultWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    Init();
}

UIYL_ManRobotRuleSearchResultWidget::~UIYL_ManRobotRuleSearchResultWidget()
{
    UIManRobotMatchRuleTipModel::GetInstance()->UnRegisterObserver(ui.m_manRobotRuleListView);
}

void UIYL_ManRobotRuleSearchResultWidget::Init()
{
    UIManRobotMatchRuleTipModel::GetInstance()->RegisterObserver(ui.m_manRobotRuleListView);

    ui.m_manRobotRuleListView->InitData();
    connect(ui.m_manRobotRuleListView, SIGNAL(S_SetSearchResultNull(bool)), this, SLOT(R_SetSearchResultNull(bool)));
}

void UIYL_ManRobotRuleSearchResultWidget::SearchText(QString text)
{
    UIManRobotMatchRuleTipModel::GetInstance()->SearchManRobotRuleByKey(text);
}

void UIYL_ManRobotRuleSearchResultWidget::R_SetSearchResultNull(bool result)
{
    if (!result)
    {
        if (ui.widget_bg->styleSheet().indexOf("image:url") != -1)
        {
            QString strStyle = "QWidget#widget_bg{border:1px solid #dddddd;background-color:rgb(255,255,255);}";
            ui.widget_bg->setStyleSheet(strStyle);
        }
    }
    else
    {
        if (ui.widget_bg->styleSheet().indexOf("image:url") == -1)
        {
            QString strStyle = "QWidget#widget_bg{border:1px solid #dddddd;background-color:rgb(255,255,255);";
            strStyle += "image:url(:/YL/Resources/YL/MainTree/icon_searchResultNull.png);}";
            ui.widget_bg->setStyleSheet(strStyle);
        }
    }
}
