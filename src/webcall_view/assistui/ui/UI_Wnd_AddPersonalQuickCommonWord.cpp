#include "UI_Wnd_AddPersonalQuickCommonWord.h"
#include "model\YL_Model\UIYL_PersonalQuickCommonWordModel.h"

UI_Wnd_AddPersonalQuickCommonWord::UI_Wnd_AddPersonalQuickCommonWord(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    this->setWindowIcon(QIcon(":/YL/Resources/YL/CommonWord/icon_addCommonword.png"));
    this->setWindowTitle(tr("添加个人常用语快捷咨询"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    UIYL_PersonalQuickCommonWordModel::GetInstance()->RegisterObserver(ui.m_addPersonalWidget);
    UIYL_PersonalQuickCommonWordModel::GetInstance()->InitData();
    connect(ui.m_addPersonalWidget, SIGNAL(S_OnClose()), this, SLOT(R_OnClose()));
}

UI_Wnd_AddPersonalQuickCommonWord::~UI_Wnd_AddPersonalQuickCommonWord()
{
}

void UI_Wnd_AddPersonalQuickCommonWord::Modify(bool val)
{
    m_isModify = val;
    ui.m_addPersonalWidget->IsModify(val);
    if (m_isModify)
    {
        this->setWindowTitle(tr("修改个人常用语快捷咨询"));
    }
    else
    {
        this->setWindowTitle(tr("添加个人常用语快捷咨询"));
    }
}

bool UI_Wnd_AddPersonalQuickCommonWord::Modify() const
{
    return m_isModify;
}

void UI_Wnd_AddPersonalQuickCommonWord::SetPersonalQuickItemData(MPersonalQuickCommonWordData *data)
{
    ui.m_addPersonalWidget->SetPersonalQuickItemData(data);
}

void UI_Wnd_AddPersonalQuickCommonWord::R_OnClose()
{
    this->close();
}
