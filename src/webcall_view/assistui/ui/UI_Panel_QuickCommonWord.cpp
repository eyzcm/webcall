#include "UI_Panel_QuickCommonWord.h"
#include "model\YL_Model\UI_Model_QuickCommonWord.h"

UI_Panel_QuickCommonWord::UI_Panel_QuickCommonWord(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setMouseTracking(true);

    Init();
}

UI_Panel_QuickCommonWord::~UI_Panel_QuickCommonWord()
{
}

void UI_Panel_QuickCommonWord::Init()
{
    UI_Model_QuickCommonWord::GetInstance()->RegisterObserver(ui.m_listView_quickCommonWord);
    this->setMinimumHeight(24 * 7);
}

void UI_Panel_QuickCommonWord::enterEvent(QEvent *event)
{
    Q_EMIT S_ShowBtn_Collapse(true);
}

void UI_Panel_QuickCommonWord::leaveEvent(QEvent *event)
{
    Q_EMIT S_ShowBtn_Collapse(false);
}
