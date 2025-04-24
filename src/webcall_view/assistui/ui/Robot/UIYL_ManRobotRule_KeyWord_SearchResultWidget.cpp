#include "UIYL_ManRobotRule_KeyWord_SearchResultWidget.h"
#include "model/UIManRobotMatchRuleTipModel.h"

UIYL_ManRobotRule_KeyWord_SearchResultWidget::UIYL_ManRobotRule_KeyWord_SearchResultWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    Init();
}

UIYL_ManRobotRule_KeyWord_SearchResultWidget::~UIYL_ManRobotRule_KeyWord_SearchResultWidget()
{
    UIManRobotMatchRuleTipModel::GetInstance()->UnRegisterObserver(ui.m_manRobot_KeyWord_RuleListView);
}

void UIYL_ManRobotRule_KeyWord_SearchResultWidget::Init()
{
    UIManRobotMatchRuleTipModel::GetInstance()->RegisterObserver(ui.m_manRobot_KeyWord_RuleListView);

    ui.m_manRobot_KeyWord_RuleListView->SetListViewType(2);
    ui.m_manRobot_KeyWord_RuleListView->InitData();
    connect(ui.m_manRobot_KeyWord_RuleListView, SIGNAL(S_SetSearchResultNull(bool)), this, SLOT(R_SetSearchResultNull(bool)));
}

void UIYL_ManRobotRule_KeyWord_SearchResultWidget::SearchText(QString text)
{
    UIManRobotMatchRuleTipModel::GetInstance()->SearchManRobotUserRuleByKey(text);
}

void UIYL_ManRobotRule_KeyWord_SearchResultWidget::R_SetSearchResultNull(bool result)
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
