#include "UI_Panel_AddCommonWord.h"
#include "UI_Widget_SystemPrompt.h"
#include "UI_Wnd_HotKey.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "utils/utilshelper.h"

UI_Panel_AddCommonWord::UI_Panel_AddCommonWord(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(ui.m_addBtn, SIGNAL(clicked()), this, SLOT(R_OnAddCommonWordBtnClick()));
    connect(ui.m_retrySettingBtn, SIGNAL(clicked()), this, SLOT(R_OnRetrySettingBtnClick()));
    connect(ui.m_deleteHotKey, SIGNAL(clicked()), this, SLOT(R_OnDeleteHotKey()));
    connect(this, SIGNAL(S_AddCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_AddCommonWord(YL_TreeViewItem_CommonWord *, UICommonWordInfo *)));
    connect(this, SIGNAL(S_ShowTipMessage(QString)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_TipMessage(QString)));
    connect(ui.m_ueditor, &el_TextEdit::S_GetInputValue, this, &UI_Panel_AddCommonWord::R_GetInputValue);
}

UI_Panel_AddCommonWord::~UI_Panel_AddCommonWord()
{
}

void UI_Panel_AddCommonWord::SetParentItem(YL_TreeViewItem_CommonWord *itemObj)
{
    m_pItemObj = itemObj;
}

void UI_Panel_AddCommonWord::ShowToastMsg(QString msg, EYLUI_ToastType type, int showTime, EYLUI_ToastShowPos showPos, QPoint tarPos)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

void UI_Panel_AddCommonWord::R_OnAddCommonWordBtnClick()
{
    //常用语添加
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

void UI_Panel_AddCommonWord::R_GetInputValue(QString str, int i)
{
    Q_UNUSED(i);
    QString content = str;
    if (content == "" || content.isEmpty())
    {
        Q_EMIT S_ShowTipMessage(tr("常用语内容不能为空！"));
    }
    else
    {
        QString hotkey = ui.m_hotkeyLineEdit->text().trimmed();
        QString uuid   = UtilsHelper::GetUUid();

        UICommonWordInfo *info = gUICommonWordManager->CreateInfo();
        info->Content(content);
        info->Id(uuid);
        info->Title(title);
        info->HotKey(hotkey);
        info->IsDir(false);
        info->Pid(m_pItemObj->ID());
        gUICommonWordManager->LazyFreeObj(info);

        Q_EMIT S_AddCommonWord(m_pItemObj, info);
        this->parentWidget()->close();
    }
}

void UI_Panel_AddCommonWord::R_OnRetrySettingBtnClick()
{
    UI_Wnd_HotKey *wnd = new UI_Wnd_HotKey;
    connect(wnd, SIGNAL(S_AddHotKey(QString)), this, SLOT(R_OnAddHotKey(QString)));
    wnd->exec();
}

void UI_Panel_AddCommonWord::R_OnDeleteHotKey()
{
    ui.m_hotkeyLineEdit->setText("");
}

void UI_Panel_AddCommonWord::R_OnAddHotKey(QString key)
{
    ui.m_hotkeyLineEdit->setText(key);
}
