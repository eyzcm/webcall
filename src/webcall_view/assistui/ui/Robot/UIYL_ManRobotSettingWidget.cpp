#include "UIYL_ManRobotSettingWidget.h"
#include "control/CommonSignalService.h"
#include "model/UIManRobotMatchRuleTipModel.h"
#include "protocol_biz/webcallprotocolservice.h"
#include "service/DbUiWebcallService.h"

UIYL_ManRobotSettingWidget::UIYL_ManRobotSettingWidget(QWidget *parent) : QWidget(parent), m_pAddNewRule(nullptr)
{
    ui.setupUi(this);
	
	
	connect(ui.m_newRuleBtn, SIGNAL(clicked()), this, SLOT(R_OnNewRuleBtnClicked()));
	connect(ui.m_saveRuleListBtn, SIGNAL(clicked()), this, SLOT(R_OnSaveRuleListBtnClicked()));
	ui.m_searchLine->installEventFilter(this);
	ui.m_saveRuleListBtn->setEnabled(false);

	m_manRobotRuleSearchResultWidget = new UIYL_ManRobotRuleSearchResultWidget(this);

	connect(ui.m_searchLine, SIGNAL(textChanged(const QString &)), this, SLOT(R_OnManRobotRuleListSearchLineTextChanged(const QString &)));
	

	connect(CommonSignalService::GetInstance(), SIGNAL(S_OnClickManRobotRuleSearchItem(QString)), this, SLOT(R_OnClickSearchItem(QString)));

	connect(ui.m_manRobotRuleListView, SIGNAL(S_DragItemChanged()), this, SLOT(R_DragItemChanged()));
	m_manRobotRuleSearchResultWidget->hide();

	ui.m_ruleWidget->SetWidgetType(1);
	UIManRobotMatchRuleTipModel::GetInstance()->RegisterObserver(ui.m_manRobotRuleListView);
	UIManRobotMatchRuleTipModel::GetInstance()->ReInitPersonalManRobotList();

	connect(this, SIGNAL(S_ShowToastMsg_AISettingCenter(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)), CommonSignalService::GetInstance(), SIGNAL(S_ShowToastMsg_AISettingCenter(QString, int, EYLUI_ToastShowPos, EYLUI_ToastType, QPoint)));
    QLocale locale;
    if (locale.language() == QLocale::Chinese && gUISystemConfigManager->GetSystemConfigByKey("language", "0") == "0")
    {
    }
    else
    {
        ui.widget_2_2_rule->setStyleSheet("QWidget#widget_2_2_rule{image: url(:/YL/Resources/YL/Robot/icon_noContent_en.png);}");
    }

}

UIYL_ManRobotSettingWidget::~UIYL_ManRobotSettingWidget()
{
    UIManRobotMatchRuleTipModel::GetInstance()->UnRegisterObserver(ui.m_manRobotRuleListView);
}

void UIYL_ManRobotSettingWidget::R_OnNewRuleBtnClicked()
{
    if (!m_pAddNewRule)
    {
        m_pAddNewRule = new BaseWnd;
        m_pAddNewRule->HideMaxBtn();
        m_pAddNewRule->HideMinBtn();
        m_pAddNewRule->SetDragEnable(true);
        m_pAddNewRule->setWindowIcon(QIcon(":/YL/Resources/YL/MainTree/ai_offline.png"));
        m_pAddNewRule->SetTitlePanelIcon(":/YL/Resources/YL/MainTree/ai_offline.png");
        m_pAddNewRule->SetWndTitle(tr("新增"));
        m_pAddNewRule->SetCloseType(4);
    }

    if (m_pAddNewRule->HasCenterWidget())
    {
        m_pAddNewRule->activateWindow();
    }
    else
    {
        UIYL_ManRobotRuleSettingWidget *widget = new UIYL_ManRobotRuleSettingWidget();
        widget->SetWidgetType(0);
        connect(widget, SIGNAL(S_HideNewAddWidget()), this, SLOT(R_HideNewAddWidget()));
        m_pAddNewRule->AddContentWidget(widget);
        QPoint pos = this->mapToGlobal(ui.widget_2_2_rule->pos());
        m_pAddNewRule->resize(580, 580);
        m_pAddNewRule->move(pos.x() - 10, pos.y() - 40);
        m_pAddNewRule->show();
    }
}

void UIYL_ManRobotSettingWidget::R_OnSaveRuleListBtnClicked()
{
    //保存排序id列表。
    QStringList idList = ui.m_manRobotRuleListView->GetIdList();

    gIProtocolAgent->DoSetAiRuleSort(idList.join(","));

    ui.m_saveRuleListBtn->setEnabled(false);

    Q_EMIT S_ShowToastMsg_AISettingCenter(tr("规则列表保存成功！"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
}

bool UIYL_ManRobotSettingWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::FocusIn)
    {
        if (obj == ui.m_searchLine)
        {
            QString text = ui.m_searchLine->text().trimmed();
            R_OnManRobotRuleListSearchLineTextChanged(text);
        }
    }
    else if (e->type() == QEvent::FocusOut)
    {
        if (obj == ui.m_searchLine)
        {
            HideWnd();
        }
    }
    else if (e->type() == QEvent::KeyPress)
    {
        if (obj == ui.m_searchLine)
        {
            QString text = ui.m_searchLine->text().trimmed();
            R_OnManRobotRuleListSearchLineTextChanged(text);
        }
    }

    return QWidget::eventFilter(obj, e);
}

void UIYL_ManRobotSettingWidget::ShowSearchWnd(QString text)
{
    if (m_manRobotRuleSearchResultWidget == nullptr)
    {
        m_manRobotRuleSearchResultWidget = new UIYL_ManRobotRuleSearchResultWidget(ui.widget_up);
    }

    int nX = ui.widget_0_content->x() + ui.widget_2_rule->x() + ui.widget_2_1_list->x() + ui.widget_up->x() + ui.m_manRobotRuleListView->x();
    int nY = ui.widget_0_content->y() + ui.widget_2_rule->y() + ui.widget_2_1_list->y() + ui.widget_up->y() + ui.m_manRobotRuleListView->y();

    m_manRobotRuleSearchResultWidget->SearchText(text);
    m_manRobotRuleSearchResultWidget->setGeometry(nX, nY, ui.m_manRobotRuleListView->width(), ui.m_manRobotRuleListView->height());
    m_manRobotRuleSearchResultWidget->show();
    m_manRobotRuleSearchResultWidget->activateWindow();
}

void UIYL_ManRobotSettingWidget::HideWnd()
{
    if (m_manRobotRuleSearchResultWidget)
    {
        m_manRobotRuleSearchResultWidget->hide();
    }
}

void UIYL_ManRobotSettingWidget::R_OnManRobotRuleListSearchLineTextChanged(const QString &text)
{
    if (text.isEmpty())
    {
        HideWnd();
    }
    else
    {
        ShowSearchWnd(text);
    }
}

void UIYL_ManRobotSettingWidget::R_OnClickSearchItem(QString id)
{
    if (m_manRobotRuleSearchResultWidget)
    {
        m_manRobotRuleSearchResultWidget->hide();
    }

    if (ui.m_searchLine)
    {
        ui.m_searchLine->clearFocus();
    }
}

void UIYL_ManRobotSettingWidget::R_DragItemChanged()
{
    ui.m_saveRuleListBtn->setEnabled(true);
}

void UIYL_ManRobotSettingWidget::R_HideNewAddWidget()
{
    if (m_pAddNewRule)
    {
        m_pAddNewRule->DeleteCenterWidget();
        m_pAddNewRule->repaint();
        m_pAddNewRule->hide();
    }
}
