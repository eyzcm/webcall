#include "UI_Wnd_ModifyCommonWord.h"
#include "model\YL_Model\UI_Model_ModifyCommonWord.h"

UI_Wnd_ModifyCommonWord::UI_Wnd_ModifyCommonWord(YL_TreeViewItem_CommonWord* itemObj, QWidget* parent) : QDialog(parent), m_pItemObj(itemObj)
{
    ui.setupUi(this);
    this->setWindowTitle(tr("修改常用语"));
    this->setWindowIcon(QIcon(":/YL/Resources/YL/CommonWord/icon_modifyCommonWord.png"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui.panel_ModifyCommonWord->Init(m_pItemObj);

    UI_Model_ModifyCommonWord::GetInstance()->RegisterObserver(ui.panel_ModifyCommonWord);
    // UI_Model_ModifyCommonWord::GetInstance()->InitData();
}

UI_Wnd_ModifyCommonWord::~UI_Wnd_ModifyCommonWord()
{
}
