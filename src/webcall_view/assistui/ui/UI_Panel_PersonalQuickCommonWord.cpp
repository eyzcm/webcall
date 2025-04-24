#include "UI_Panel_PersonalQuickCommonWord.h"
#include "model\YL_Model\UIYL_PersonalQuickCommonWordModel.h"

UI_Panel_PersonalQuickCommonWord::UI_Panel_PersonalQuickCommonWord(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setMouseTracking(true);

    Init();
}

UI_Panel_PersonalQuickCommonWord::~UI_Panel_PersonalQuickCommonWord()
{
}

void UI_Panel_PersonalQuickCommonWord::Init()
{
    UIYL_PersonalQuickCommonWordModel::GetInstance()->RegisterObserver(ui.m_listView);
    this->setMinimumHeight(24 * 7);
}

void UI_Panel_PersonalQuickCommonWord::enterEvent(QEvent *event)
{
    Q_EMIT S_ShowBtn_Collapse(true);
}

void UI_Panel_PersonalQuickCommonWord::leaveEvent(QEvent *event)
{
    Q_EMIT S_ShowBtn_Collapse(false);
}
