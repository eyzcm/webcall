#include "uiyl_editfunother.h"

UIYL_EditFunOther::UIYL_EditFunOther()
{

}

void UIYL_EditFunOther::Init()
{



    connect(this, SIGNAL(S_OnSendFile(EMESSAGETYPE, QString, EMESSAGETYPE)), CommonSignalService::GetInstance(), SIGNAL(S_OnSendFile(EMESSAGETYPE, QString, EMESSAGETYPE)));
}

void UIYL_EditFunOther::InitRegisterSignal()
{
    connect(gSessionService->LoginSignal(), SIGNAL(S_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)), this, SLOT(R_LoginStateChanged(ELOGINSTATUS, ELOGINSTATUS)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_SendAutoRobotReplyMessage(MChatItemData *, QString, bool, QString, CommInfo)), this, SLOT(R_SendAutoRobotReplyMessage(MChatItemData *, QString, bool, QString, CommInfo)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnCommonWordTreeItemClick(MCommonWordData *)), this, SLOT(R_OnCommonWordTreeItemClick(MCommonWordData *)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnCommonWordTreeItemDoubleClick(MCommonWordData *)), this, SLOT(R_OnCommonWordTreeItemDoubleClick(MCommonWordData *)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnDoubleClick_PersonalQuickCommonWord(MPersonalQuickCommonWordData *)), this, SLOT(R_OnDoubleClick_PersonalQuickCommonWord(MPersonalQuickCommonWordData *)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnFontChanged(MFontData *)), this, SLOT(R_OnFontChanged(MFontData *)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ReturnPersonSetData(QString, QString)), this, SLOT(R_ReturnPersonSetData(QString, QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_AddReferredMsg(QString)), this, SLOT(R_AddReferredMsg(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OtherSettingChanged()), this, SLOT(R_ShowSettingChanged()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ShowSettingChanged()), this, SLOT(R_ShowSettingChanged()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OpertorSettingChanged()), this, SLOT(R_ShowSettingChanged()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnCommonWordListItemClick(MCommonWordData *)), this, SLOT(R_OnCommonWordListItemClick(MCommonWordData *)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_AddEditText(QString)), this, SLOT(R_AddEditText(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnInsertWeiXin(QString, QString)), this, SLOT(R_OnInsertWeiXin(QString, QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_RemoveFormat()), this, SLOT(R_RemoveFormat()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_SendTable_FormatData(QString)), this, SLOT(R_SendTable_FormatData(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ReSendFile(QString)), ui.m_my_chat_toolBarPanel, SLOT(R_ReSendFile(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ScreenShotFinish()), this, SLOT(R_ScreenShotFinish()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OnSelectedChatItemData(MChatItemData *)), this, SLOT(R_OnSelectedChatItemData(MChatItemData *)));

    connect(CommonSignalService::GetInstance(), SIGNAL(S_SelectEmo(QString)), this, SLOT(R_SelectEmo(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_ReplyComments(QString)), this, SLOT(R_ReplyComments(QString))); //触发视频评论回复弹框
    connect(gIProtocolAgent, SIGNAL(S_GetUserPermissionListResult(QString, QString, QString)), this, SLOT(R_GetUserPermissionListResult(QString, QString, QString)));
}




bool UIYL_EditFunOther::IsCheckSendMessage(MMonitorChatData *item)
{
    if (item == NULL)
    {
        return false;
    }
    return true;
}

bool UIYL_EditFunOther::IsCheckSendMessage(MChatItemData *item)
{
    if (item == NULL)
    {
        return false;
    }
    if (item->ChatState() == E_CHATITEM_CHAT)
    {
        if (item->UserType() == E_VISITOR)
        {
            QString chatId = item->Chatid();
            if (chatId.isEmpty())
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else if (item->UserType() == E_CUSTOMER)
        {
            UIContactInfo *info = UICommonDataModel::GetInstance()->GetColleagueMemberByID(item->Vid());
            if (info)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else if (item->ChatState() == E_CHATITEM_MONITOR_CHAT)
    {
        //监控对话能发消息
        return true;
    }
    //增加邀请发送文字 zcmadd
    else if (item->ChatState() == E_CHATITEM_INVITE)
    {
        return true;
    }
    else if (item->ChatState() == E_CHATITEM_BROWSE)
    {
        return true;
    }
    return false;
}

MChatItemData *UIYL_EditFunOther::CurrentChatItem()
{
    return UIYL_ChatRecordModel::GetInstance()->CurrentChatItem();
}

void UIYL_EditFunOther::R_SendAutoRobotReplyMessage(MChatItemData *objData, QString message, bool isJoin, QString actionId, CommInfo otherInfo)
{
    if (isJoin)
    {
        MChatItemData *currChatItem = CurrentChatItem();
        if (currChatItem && objData && currChatItem->Chatid() == objData->Chatid())
        {
            ui.m_webView_edit->GetInputValue(2);
        }
    }
}

void UIYL_EditFunOther::R_OnSelectedChatItemData(MChatItemData *currObj)
{
    if (UIYL_ChatRecordModel::IsUserType(currObj))
    {
        if (!currObj->Chatid().isEmpty())
        {
            UIChatInfo *info = UICommonDataModel::GetInstance()->GetChatByChatId(currObj->Chatid());
            if (info)
            {
                if (m_chatJoinMap.find(currObj->Chatid()) == m_chatJoinMap.end())
                {
                    ui.m_robot_joinInBtn->show();
                    ui.m_robot_returnBtn->hide();
                    ui.m_btn_alternateRobot_showOrHide->hide();
                }
                else
                {
                    AI_CHAT_STATUS type = m_chatJoinMap[currObj->Chatid()];
                    switch (type)
                    {
                        case CHAT_STATUS_NONE:
                            ui.m_robot_joinInBtn->show();
                            ui.m_robot_returnBtn->hide();
                            ui.m_btn_alternateRobot_showOrHide->hide();
                            break;
                        case CHAT_STATUS_ROBOT:
                        case CHAT_STATUS_JOININ:
                            ui.m_robot_joinInBtn->show();
                            ui.m_robot_returnBtn->hide();
                            ui.m_btn_alternateRobot_showOrHide->show();
                            ShowAlternateRobot();
                            break;
                        case CHAT_STATUS_JOINOVER:
                            ui.m_robot_joinInBtn->hide();
                            ui.m_robot_returnBtn->show();
                            ui.m_btn_alternateRobot_showOrHide->show();
                            ShowAlternateRobot();
                            break;
                    }
                }

                if (info->CustomerID() == gSessionService->UserInfo()->UserId())
                {
                    ui.m_transchatInBtn->hide();
                }
                else
                {
                    //获取抢接对话权限，根据权限设置是否显示抢接对话按钮
                    bool seizeChat = gSessionService->HasPermissionByKey("seizeChat");
                    if (seizeChat)
                    {
                        //显示抢接按钮
                        ui.m_transchatInBtn->show();
                    }
                }
            }
        }
        else
        {
            if (m_chatJoinMap.find(currObj->Chatid()) == m_chatJoinMap.end())
            {
                ui.m_robot_joinInBtn->show();
                ui.m_robot_returnBtn->hide();
            }
            else
            {
                ui.m_robot_joinInBtn->hide();
                ui.m_robot_returnBtn->show();
            }
            ui.m_transchatInBtn->hide();
        }
    }
    R_OnChatItemSelectChanged(nullptr, currObj);
}

void UIYL_EditFunOther::R_OnChatItemSelectChanged(MChatItemData *oldObj, MChatItemData *currObj)
{
    Q_UNUSED(oldObj);
    if (currObj)
    {
        //候补机器人
        if (m_currentItem != currObj)
        {
            m_currentItem = currObj;
        }

        if (m_currentVid != currObj->Vid())
        {
            if (!m_currentVid.isEmpty())
            {
                ui.m_webView_edit->GetInputValue(3);
            }
            else
            {
                m_currentVid       = currObj->Vid();
                QString id         = currObj->GetExtendData("SearchEngineId");
                QString searchName = gUISystemConfigManager->GetEngineNameById(id);
                // CZC：此处改为webView方式，不用禁用，因为不会抢焦点并影响到快捷键
                ui.m_webView_edit->SetInputValue(m_inputTextMap[m_currentVid]);
                m_referredMsg = m_inputRefeererMap[m_currentVid];
                ui.m_webView_edit->AddReferredMsg(m_referredMsg);
                ui.m_webView_edit->SetSearchEngineName(searchName);
                ui.m_webView_edit->setFocus();
            }
        }

        //todo 列新工具栏显示
        ui.m_my_chat_toolBarPanel.OnChatItemSelectChanged(oldObj,currObj);
        //设置中心禁用了人工接管，窗口添加遮罩，所有功能都不能使用
        ShowMask();
    }
    else
    {
        m_currentVid  = "";
        m_currentItem = nullptr;
    }
}









void UIYL_EditFunwidget::R_OnCommonWordTreeItemDoubleClick(MCommonWordData *dataObj)
{
    bool hsManualTakeOver = gSessionService->HasPermissionByKey("b512243a-ec37-4207-8190-fd6cc26dbd98");
    if (!hsManualTakeOver)
    {
        UIOpertorSettingInfo *info = gUIPersonalConfigYLService->OpertorSettingInfo();
        if (info->DoubleClickCommonWord() == 1)
        {
            SendMessageByBody(dataObj->Content());
        }
        else
        {
            SetCommonWordContentToEdit(dataObj);
        }
    }
}



