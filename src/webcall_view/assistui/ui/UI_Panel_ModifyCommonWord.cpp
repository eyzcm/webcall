#include "UI_Panel_ModifyCommonWord.h"
#include "UI_Widget_SystemPrompt.h"
#include "UI_Wnd_HotKey.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"

UI_Panel_ModifyCommonWord::UI_Panel_ModifyCommonWord(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    ui.m_stackedWidget->setCurrentIndex(0);
    m_dataInfo = new UICommonWordInfo();
    m_pItem    = nullptr;
    connect(ui.m_modifyBtn, SIGNAL(clicked()), this, SLOT(R_OnModifyBtnClick()));
    connect(ui.m_retrySettingBtn, SIGNAL(clicked()), this, SLOT(R_OnRetrySettingBtnClick()));
    connect(ui.m_deleteHotKey, SIGNAL(clicked()), this, SLOT(R_OnDeleteHotKey()));
    connect(this, SIGNAL(S_ModifyCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_ModifyCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)));
    connect(this, SIGNAL(S_ShowTipMessage(QString)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_TipMessage(QString)));
    connect(ui.m_ueditor, &el_TextEdit::S_GetInputValue, this, &UI_Panel_ModifyCommonWord::R_GetInputValue);
}

UI_Panel_ModifyCommonWord::~UI_Panel_ModifyCommonWord()
{
}

void UI_Panel_ModifyCommonWord::Init(YL_TreeViewItem_CommonWord *item)
{
    if (item)
    {
        m_pItem = item;

        YL_TreeViewItemLeafNodeData_CommonWord *itemData = (YL_TreeViewItemLeafNodeData_CommonWord *)item->Ptr();
        if (itemData)
        {
            ui.m_titleLineEdit->setText(itemData->Title());
            ui.m_hotkeyLineEdit->setText(itemData->HotKey());
            ui.m_ueditor->SetInputTxt(itemData->Content());
        }
    }
}

void UI_Panel_ModifyCommonWord::ShowToastMsg(QString msg, EYLUI_ToastType type, int showTime, EYLUI_ToastShowPos showPos, QPoint tarPos)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

void UI_Panel_ModifyCommonWord::R_OnModifyBtnClick()
{
    title = ui.m_titleLineEdit->text().trimmed();
    if (title.isEmpty())
    {
        Q_EMIT S_ShowTipMessage(tr("常用语标题不能为空！"));
    }
    else
    {
        ui.m_ueditor->GetInputTxt();
    }
}

void UI_Panel_ModifyCommonWord::R_GetInputValue(QString str, int i)
{
    Q_UNUSED(i);
    QString content = str;
    if (content.isEmpty())
    {
        Q_EMIT S_ShowTipMessage(tr("常用语内容不能为空！"));
    }
    else
    {
        QString hotkey = ui.m_hotkeyLineEdit->text().trimmed();
        m_dataInfo->Title(title);
        m_dataInfo->Content(content);
        m_dataInfo->HotKey(hotkey);
        Q_EMIT S_ModifyCommonWord(m_pItem, m_dataInfo);
        this->parentWidget()->close();
    }
}

void UI_Panel_ModifyCommonWord::R_OnRetrySettingBtnClick()
{
    UI_Wnd_HotKey *wnd = new UI_Wnd_HotKey;
    connect(wnd, SIGNAL(S_AddHotKey(QString)), this, SLOT(R_OnAddHotKey(QString)));
    wnd->exec();
}

void UI_Panel_ModifyCommonWord::R_OnDeleteHotKey()
{
    ui.m_hotkeyLineEdit->setText("");
}

void UI_Panel_ModifyCommonWord::R_OnAddHotKey(QString key)
{
    ui.m_hotkeyLineEdit->setText(key);
}
