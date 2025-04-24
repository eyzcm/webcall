#include "UI_Panel_AddPersonalQuickCommonWord.h"
#include "UI_Widget_SystemPrompt.h"
#include "UI_Wnd_HotKey.h"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "utils/utilshelper.h"

UI_Panel_AddPersonalQuickCommonWord::UI_Panel_AddPersonalQuickCommonWord(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    m_nSortId    = 0;
    m_modifyItem = nullptr;

    ui.hotKeyWidget->setVisible(false);

    connect(ui.m_cancelBtn, SIGNAL(clicked()), this, SLOT(R_OnClose()));
    connect(ui.m_sureBtn, SIGNAL(clicked()), this, SLOT(R_OnAddCommonWordBtnClick()));
    connect(ui.m_retrySettingBtn, SIGNAL(clicked()), this, SLOT(R_OnRetrySettingBtnClick()));
    connect(ui.m_deleteHotKey, SIGNAL(clicked()), this, SLOT(R_OnDeleteHotKey()));
    connect(ui.m_add_btnItem, SIGNAL(clicked()), this, SLOT(R_OnAddButtonItemSettingClick()));

    connect(this, SIGNAL(S_AddPersonalQuickCommonWord(MPersonalQuickCommonWordData *, bool)), CommonSignalService::GetInstance(), SIGNAL(S_AddPersonalQuickCommonWord(MPersonalQuickCommonWordData *, bool)));

    connect(this, SIGNAL(S_ShowTipMessage(QString)), CommonSignalService::GetInstance(), SIGNAL(S_CommonWord_TipMessage(QString)));
    connect(ui.m_ueditor, &el_TextEdit::S_GetInputValue, this, &UI_Panel_AddPersonalQuickCommonWord::R_GetInputValue);
}

UI_Panel_AddPersonalQuickCommonWord::~UI_Panel_AddPersonalQuickCommonWord()
{
    if (m_modifyItem)
    {
        m_modifyItem->deleteLater();
        m_modifyItem = nullptr;
    }
}

void UI_Panel_AddPersonalQuickCommonWord::ShowToastMsg(QString msg, EYLUI_ToastType type, int showTime, EYLUI_ToastShowPos showPos, QPoint tarPos)
{
    UI_Widget_SystemPrompt *pWidget_SystemPtompt = new UI_Widget_SystemPrompt(this);
    pWidget_SystemPtompt->SetMsg(msg, type);
    pWidget_SystemPtompt->SetShowTime(showTime);
    pWidget_SystemPtompt->SetShowPos(this, showPos, this->mapFromGlobal(tarPos));
    pWidget_SystemPtompt->show();
}

void UI_Panel_AddPersonalQuickCommonWord::R_OnAddCommonWordBtnClick()
{
    //常用语添加
    title = ui.m_titleLineEdit->text().trimmed();
    if (title.isEmpty())
    {
        Q_EMIT S_ShowTipMessage(tr("个人常用语快捷咨询标题不能为空！"));
    }
    else
    {
        ui.m_ueditor->GetInputTxt();
    }
}

void UI_Panel_AddPersonalQuickCommonWord::R_GetInputValue(QString str, int i)
{
    Q_UNUSED(i);
    QString content = str;
    if (content == "" || content.isEmpty())
    {
        Q_EMIT S_ShowTipMessage(tr("个人常用语快捷咨询内容不能为空！"));
        // ShowToastMsg("常用语内容不能为空！", 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
    }
    else if (title.length() > 20)
    {
        Q_EMIT S_ShowTipMessage(tr("个人常用语快捷咨询标题不能超过20个，请调整后发送！"));
    }
    else
    {
        QString hotkey = ui.m_hotkeyLineEdit->text().trimmed();
        QString uuid   = UtilsHelper::GetUUid();
        if (m_modifyItem)
        {
            uuid = m_modifyItem->Id();
        }

        MPersonalQuickCommonWordData *info = new MPersonalQuickCommonWordData();
        info->Content(content);
        info->Id(uuid);
        info->Title(title);
        info->HotKey(hotkey);

        bool isOk = true;

        for (auto itor = m_sortIdList.begin(); itor != m_sortIdList.end(); itor++)
        {
            QString                  id      = itor.value();
            UI_Panel_BtnItemSetting *btnItem = m_btnItemSettingList[id];
            if (btnItem)
            {
                if (!btnItem->GetButtonString().isEmpty())
                {
                    CommInfo item;
                    item.SetValueByKey("id", UtilsHelper::GetUUid());
                    item.SetValueByKey("name", btnItem->GetButtonString());
                    item.SetValueByKey("color", btnItem->GetButtonColor());
                    item.SetValueByKey("sortNumber", QString::number(itor.key()));
                    info->AddQuickButtonItem(item);
                }
                else
                {
                    isOk = false;
                    btnItem->SetItemRedBorder(true);
                }
            }
        }
        if (!isOk)
        {
            Q_EMIT S_ShowTipMessage(tr("请输入按钮文案！"));
            info->deleteLater();
            return;
        }
        Q_EMIT S_AddPersonalQuickCommonWord(info, isModify);
        this->parentWidget()->close();
    }
}

void UI_Panel_AddPersonalQuickCommonWord::R_OnRetrySettingBtnClick()
{
    UI_Wnd_HotKey *wnd = new UI_Wnd_HotKey;
    connect(wnd, SIGNAL(S_AddHotKey(QString)), this, SLOT(R_OnAddHotKey(QString)));
    wnd->exec();
}

void UI_Panel_AddPersonalQuickCommonWord::R_OnDeleteHotKey()
{
    ui.m_hotkeyLineEdit->setText("");
}

void UI_Panel_AddPersonalQuickCommonWord::R_OnAddHotKey(QString key)
{
    ui.m_hotkeyLineEdit->setText(key);
}

void UI_Panel_AddPersonalQuickCommonWord::R_OnAddButtonItemSettingClick()
{
    if (m_btnItemSettingList.size() > 7)
    {
        Q_EMIT S_ShowTipMessage(tr("访客可快速点击咨询相关信息的按钮，最多可添加8个。"));
        return;
    }
    m_nSortId++;

    QString                  uuid           = UtilsHelper::GetUUid();
    UI_Panel_BtnItemSetting *btnItemSetting = new UI_Panel_BtnItemSetting;
    btnItemSetting->SetUUID(uuid);
    connect(btnItemSetting, SIGNAL(S_OnRemoveBtnClick(QString)), this, SLOT(R_OnRemoveBtnClick(QString)));
    connect(btnItemSetting, SIGNAL(S_ShowTipMessage(QString)), this, SIGNAL(S_ShowTipMessage(QString)));
    ui.gridLayout_BtnItem->addWidget(btnItemSetting);
    m_btnItemSettingList.insert(uuid, btnItemSetting);
    m_sortIdList.insert(m_nSortId, uuid);
}

void UI_Panel_AddPersonalQuickCommonWord::R_OnRemoveBtnClick(QString uuid)
{
    if (m_btnItemSettingList.find(uuid) != m_btnItemSettingList.end())
    {
        UI_Panel_BtnItemSetting *item = m_btnItemSettingList[uuid];

        m_btnItemSettingList.remove(uuid);
        if (item)
        {
            ui.gridLayout_BtnItem->removeWidget(item);

            item->deleteLater();
            item = nullptr;
        }
    }
}

void UI_Panel_AddPersonalQuickCommonWord::SetPersonalQuickItemData(MPersonalQuickCommonWordData *data)
{
    if (data)
    {
        m_modifyItem = new MPersonalQuickCommonWordData(*data);

        ui.m_titleLineEdit->setText(m_modifyItem->Title());
        ui.m_hotkeyLineEdit->setText(m_modifyItem->HotKey());
        ui.m_ueditor->SetInputTxt(m_modifyItem->Content());

        QMap<int, QString>        m_sortList = m_modifyItem->SortList();
        QMap<QString, CommInfo *> mapList    = m_modifyItem->ButtonMapList();
        for (auto itor = m_sortList.begin(); itor != m_sortList.end(); itor++)
        {
            QString   id   = itor.value();
            CommInfo *info = mapList[id];
            if (info)
            {
                m_nSortId++;
                UI_Panel_BtnItemSetting *btnItem = new UI_Panel_BtnItemSetting;
                btnItem->SetUUID(info->GetValueByKey("id"));
                btnItem->SetButtonString(info->GetValueByKey("name"));
                btnItem->SetButtonColor(info->GetValueByKey("color"));

                connect(btnItem, SIGNAL(S_OnRemoveBtnClick(QString)), this, SLOT(R_OnRemoveBtnClick(QString)));
                connect(btnItem, SIGNAL(S_ShowTipMessage(QString)), this, SIGNAL(S_ShowTipMessage(QString)));
                ui.gridLayout_BtnItem->addWidget(btnItem);
                m_btnItemSettingList.insert(info->GetValueByKey("id"), btnItem);
                m_sortIdList.insert(m_nSortId, id);
            }
        }
    }
}

void UI_Panel_AddPersonalQuickCommonWord::IsModify(bool val)
{
    if (!val)
    {
        R_OnAddButtonItemSettingClick();
    }
    isModify = val;
}

bool UI_Panel_AddPersonalQuickCommonWord::IsModify() const
{
    return isModify;
}

void UI_Panel_AddPersonalQuickCommonWord::R_OnClose()
{
    Q_EMIT S_OnClose();
}
