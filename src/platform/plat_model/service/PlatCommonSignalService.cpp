#include "stdafx.h"
#include "PlatCommonSignalService.h"
#include "platcommonmodelservice.h"
#include "session/el_sessionservice.h"
#include "data/UIVisitorUploadDataInfo.h"

#include <model/uiplatcommondatamodel.h>
#include "protocol/platprotocolservice.h"

PlatCommonSignalService::PlatCommonSignalService(void)
{
}

PlatCommonSignalService::~PlatCommonSignalService(void)
{
}



PlatCommonSignalService *PlatCommonSignalService::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new PlatCommonSignalService;
    }
    return m_instance;
}

PlatCommonSignalService *PlatCommonSignalService::m_instance = nullptr;

PlatCommonSignalService::Garbo::~Garbo()
{
    if (PlatCommonSignalService::m_instance)
    {
        delete PlatCommonSignalService::m_instance;
        PlatCommonSignalService::m_instance = NULL;
    }
}


void PlatCommonSignalService::InitSignal()
{
    //ai相关的


}

void PlatCommonSignalService::DoExpirationReminder(QString ExpirationReminder)
{
    Q_EMIT S_ExpirationReminder(ExpirationReminder);
}


void PlatCommonSignalService::DoShowAnnouncementNotice(QString title, QString content, QString time)
{
    Q_EMIT S_ShowAnnouncementNotice(title, content, time);
}


void PlatCommonSignalService::DODoQuit()
{
    Q_EMIT S_DoQuit();
}

void PlatCommonSignalService::DoGetViewEnv()
{
    Q_EMIT S_GetViewEnv();
}



/* 曹振超：2019年8月25日 */
void PlatCommonSignalService::DoClickCountChanged()
{
    Q_EMIT S_ClickCountChanged();
}

void PlatCommonSignalService::DoUploadFileEnd(QString fileType, EUSERTYPE type, QString fileName, QString msgTo, EMESSAGETYPE msgType)
{
    if (type == E_UEDITOR)
    {
        Q_EMIT S_ueUploadFileEnd(fileType, type, fileName, msgTo, msgType);
    }
    else
        Q_EMIT S_SendFileMsg("douploadfileend", type, fileName, msgTo, msgType); //发送文件完成 11
}

void PlatCommonSignalService::DoUploadingFile(QString fileType, EUSERTYPE type, QString code, QString fileName, EMESSAGETYPE msgType)
{
    Q_EMIT S_SendFileMsg(fileType, type, fileName, code, msgType); //发送文件中 1
}

void PlatCommonSignalService::DoCreateTempChat(UIChatInfo *pInfo, bool isoffline)
{
    if (isoffline)
    {
        Q_EMIT S_CreateChatoffline(pInfo);
    }
    else
    {
        Q_EMIT S_CreateTempChat(pInfo);
    }
}

void PlatCommonSignalService::DoReceiveChatCloseMsg(QString chatid, QString vid, QString closeType)
{
    Q_EMIT S_ReceiveChatCloseMsg(chatid, vid, closeType);
}

void PlatCommonSignalService::DoCreateTransferChat(UIChatInfo *pinfo, QString msgData)
{
    Q_EMIT S_CreateTransferChat(pinfo, msgData);
}

void PlatCommonSignalService::DoShowColleagueList()
{
    Q_EMIT S_ShowColleagueList();
}

void PlatCommonSignalService::DoGetCommonWordColleageListResult()
{
    Q_EMIT S_GetCommonWordColleageListResult();
}

void PlatCommonSignalService::DoSendClpboardImage(MChatItemData *chatItem)
{
    Q_EMIT S_OnSendClpboardImage(chatItem);
}

void PlatCommonSignalService::DoSetCommonWordinfo(QString id)
{
    Q_EMIT S_SetCommonWordinfo(id);
}

void PlatCommonSignalService::DoGetCurrentChatRecordFinished(QString chatid, QString body)
{
    Q_EMIT S_GetCurrentChatRecordFinished(chatid, body);
}
void PlatCommonSignalService::DoGetVisitorHistoryChat(QString vid, QString body)
{
    Q_EMIT S_GetVisitorHistoryChat(vid, body);
}

void PlatCommonSignalService::DoGetVisitorHistoryChatByDouyinOffline(QString UserId, QString body)
{
    Q_EMIT S_GetVisitorHistoryChatByDouyinOffline(UserId, body);
}

void PlatCommonSignalService::DoShowNotice(CommInfo *pInfo)
{
    Q_EMIT S_ShowNotice(pInfo);
}

void PlatCommonSignalService::DoGetVisitorColsEnd()
{
    Q_EMIT S_GetVisitorColsEnd();
}

void PlatCommonSignalService::DoGetVisitorInfoResult(QString userid, UICardInfo *info, int type)
{
    Q_EMIT S_GetVisitorInfoResult(userid, info, type);
}

void PlatCommonSignalService::DoGetWaitQueueEnd()
{
    Q_EMIT S_GetWaitQueueEnd();
}

void PlatCommonSignalService::DoCustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType)
{
    Q_EMIT S_CustomerPresenceChanged(fromId, newStatus, oldStatus, loginType);
}


void PlatCommonSignalService::DoAddWaitQueueInfo(UIWaitVisitorInfo *pinfo)
{
    Q_EMIT S_AddWaitQueueInfo(pinfo);
}

void PlatCommonSignalService::DoRemoveWaitQueueInfoByVid(QString fromId)
{
    Q_EMIT S_RemoveWaitQueueInfoByVid(fromId);
}

void PlatCommonSignalService::DoAddVisitorInfo(UIVisitorInfo *pinfo)
{
    Q_EMIT S_AddVisitorInfo(pinfo);
}

void PlatCommonSignalService::DoSetVisitorStatus(QString vid, EVISITORSTATUS newStatus, EVISITORSTATUS oldStatus)
{
    Q_EMIT S_SetVisitorStatus(vid, newStatus, oldStatus);
}

void PlatCommonSignalService::DoRemoveVMVisitorList(const QStringList &list)
{
    Q_EMIT S_RemoveVMVisitorList(list);
}

void PlatCommonSignalService::DoRemoveTempVMVisitorList(const QStringList &list)
{
    Q_EMIT S_RemoveTempVMVisitorList(list);
}

void PlatCommonSignalService::DoReceiveMessage(const Im_Message &msg)
{
    Q_EMIT S_ReceiveMessage(msg);
}

void PlatCommonSignalService::DoReceiveAIMessage(QString vid, QString chatId, QString body)
{
    Q_EMIT S_ReceiveAIMessage(vid, chatId, body);
}


void PlatCommonSignalService::DoReceived_Im_AI_Robot_Message(int type, QString chatId, QString customerId, int delaytime)
{
    Q_EMIT S_Received_Im_AI_Robot_Message(type, chatId, customerId, delaytime);
}

void PlatCommonSignalService::DoOtherSettingChanged()
{
    Q_EMIT S_OtherSettingChanged();
}

void PlatCommonSignalService::DoMessageTipSettingChanged()
{
    Q_EMIT S_MessageTipSettingChanged();
}


void PlatCommonSignalService::DoGetCompanyInfoEnd()
{
    Q_EMIT S_GetCompanyInfoEnd();
}

void PlatCommonSignalService::DoGetCompanyInfoStatisticsEnd()
{
    Q_EMIT S_GetCompanyInfoStatisticsEnd();
}

void PlatCommonSignalService::DoChatSettingChanged()
{
    Q_EMIT S_ChatSettingChanged();
}

void PlatCommonSignalService::DoAddChat(UIChatInfo info)
{

    Q_EMIT S_AddChat(info);
}

void PlatCommonSignalService::DoUpdateChat(UIChatInfo info)
{
     Q_EMIT S_UpdateChat(info);
}

void PlatCommonSignalService::DoDelChat(QString chatId, QString customerID, QMap<QString, QString> extendInfo)
{
    Q_EMIT S_DelChat(chatId, customerID, extendInfo);
}

void PlatCommonSignalService::DoAddVisitor(UIVisitorInfo info)
{
     Q_EMIT S_AddVisitor(info);
}

void PlatCommonSignalService::DoUpdateVisitor(UIVisitorInfo info)
{

      Q_EMIT S_UpdateVisitor(info);

}


void PlatCommonSignalService::DoAddTagFinished(QString tagId, QString uuid)
{
    Q_EMIT S_AddTagFinished(tagId, uuid);
}


void PlatCommonSignalService::DoUploadOtherFileEnd(QString fileType, QString jsonstr)
{
    Q_EMIT S_UploadOtherFileEnd(fileType, jsonstr);
}


void PlatCommonSignalService::DoGetChatFriendlyEnd()
{
    Q_EMIT S_GetChatFriendlyEnd();
}

void PlatCommonSignalService::DoAddFriend(UIContactInfo info)
{
    Q_EMIT S_AddFriend(info);
}

void PlatCommonSignalService::DoUpdateFriend(UIContactInfo info)
{
    Q_EMIT S_UpdateFriend(info);
}

void PlatCommonSignalService::DoDelFriend(QString uid)
{
    Q_EMIT S_DelFriend(uid);
}

void PlatCommonSignalService::DoAddChatItemData(MChatItemData *obj)
{
    Q_EMIT S_OnAddChatItemData(obj);
}

void PlatCommonSignalService::DoChatItemCloseClicked(QString vid, QString cid)
{
    Q_EMIT S_OnChatItemCloseClicked(vid, cid);
}

void PlatCommonSignalService::DoSelectedChatItemData(MChatItemData *obj)
{
    Q_EMIT S_OnSelectedChatItemData(obj);
}

void PlatCommonSignalService::DoSendMessage(MChatItemData *obj, QString type, QString messageBody)
{
    Q_EMIT S_OnSendMessage(obj, type, messageBody);
}

void PlatCommonSignalService::DoVisitorFilterChanged()
{
    Q_EMIT S_VisitorFilterChanged();
}

void PlatCommonSignalService::DoBackupColorChanged()
{
    Q_EMIT S_BackupColorChanged();
}

void PlatCommonSignalService::DoShowIeWndByUri(EURITYPE type, int openType, QString subURiParam)
{
    Q_EMIT S_ShowIeWndByUri(type, openType, subURiParam);
}
void PlatCommonSignalService::DoShowSettingChanged()
{
    Q_EMIT S_ShowSettingChanged();
}

void PlatCommonSignalService::DoCurrentSkinChanged()
{
    Q_EMIT S_CurrentSkinChanged();
}

void PlatCommonSignalService::DoOpertorSettingChanged()
{
    Q_EMIT S_OpertorSettingChanged();
}

void PlatCommonSignalService::DoAutoJoinTagBySendMsg(QString chatId)
{
    //上报访客信息
       UIVisitorUploadDataInfo *info = gSessionService->CreateUploadData();
       info->ChatId(chatId);
       info->SecType("2010");
       info->ThirdType("2010001");
       //接管前对话id Long takeOverFrontChatId、接管后对话id Long takeOverLaterChatId、接管前客服id String takeOverFrontUserId
       info->AddExtsMap("takeOverFrontChatId", chatId);
       info->AddExtsMap("takeOverLaterChatId", chatId);
       info->AddExtsMap("takeOverFrontUserId", gSessionService->UserInfo()->UserId());
       info->AddExtsMap("takeOverLaterUserId", gSessionService->UserInfo()->UserId());

       UIPlatCommonDataModel::Instance()->UpdateVisitorUpdateData(info);
       gPlatProtocolService->DoVisitorUploadData(info);
    Q_EMIT S_AutoJoinTagBySendMsg(chatId);
        delete info;
}

void PlatCommonSignalService::DoRobotAutoReplyError(MChatItemData *objData, const QString &content, QString code)
{
    Q_EMIT S_RobotAutoReplyError(objData, content, code);
}

void PlatCommonSignalService::DoRobotNoReplyError(QString chatId, QString recordId)
{
    Q_EMIT S_RobotNoReplyError(chatId, recordId);
}

void PlatCommonSignalService::DoSetState(EPRESENCESTATUS status)
{
    Q_EMIT S_DoSetState(status);
}

void PlatCommonSignalService::DoStateChange(EPRESENCESTATUS fromstate, EPRESENCESTATUS tostate)
{
    Q_EMIT S_DoStateChange(fromstate,tostate);
}

void PlatCommonSignalService::DoShowRebotList(QList<CommInfo *> &list)
{
    Q_EMIT S_ShowRebotList(list);
}

void PlatCommonSignalService::DoGetRobotAnswer(QString answer)
{
    Q_EMIT S_AddEditText(answer);
}

void PlatCommonSignalService::DoAddReferredMsg(QString jsontext)
{
    Q_EMIT S_AddReferredMsg(jsontext);
}

void PlatCommonSignalService::DoAddEditText(QString text)
{
    Q_EMIT S_AddEditText(text);
}

void PlatCommonSignalService::DoSummaryResult(QString vid, QString chatId, bool isOk)
{
    Q_EMIT S_SummaryResult(vid, chatId, isOk);
}


void PlatCommonSignalService::DoRemoveChatItem(QString vid)
{
    Q_EMIT S_RemoveChatItem(vid);
}


void PlatCommonSignalService::DoGetCompanyInfosByCompanyIdFinished(QString code, QString reason, QString body)
{
    Q_EMIT S_DoGetCompanyInfosByCompanyIdFinished(code, reason, body);
}

void PlatCommonSignalService::DoGetSummaryCategoryFinished(QString code, QString reason, QString body)
{
    Q_EMIT S_DoGetSummaryCategoryFinished(code, reason, body);
}

void PlatCommonSignalService::DoFindUserAiRulesResult(QString code, QString reason, QString body)
{
    Q_EMIT S_FindUserAiRulesResult(code, reason, body);
}

void PlatCommonSignalService::DoSetAiRuleSortResult(QString code, QString reason, QString body)
{
    Q_EMIT S_SetAiRuleSortResult(code, reason, body);
}

void PlatCommonSignalService::DoGetRobotRuleConfigListResult(QString code, QString reason, QString body)
{
    Q_EMIT S_GetRobotRuleConfigListResult(code, reason, body);
}


void PlatCommonSignalService::DoGetRobotRuleRoleConfigListResult(QString code, QString reason, QString body)
{
    Q_EMIT S_GetRobotRuleRoleConfigListResult(code, reason, body);
}

void PlatCommonSignalService::DoGetUserRuleListResult(QString code, QString reason, QString body)
{
    Q_EMIT S_GetUserRuleListResult(code, reason, body);
}

void PlatCommonSignalService::DoGetUnreadAnnouncementResult(QString code, QString reason, QString body, int count)
{
    Q_EMIT S_GetUnreadAnnouncementResult(code, reason, body, count);
}



void PlatCommonSignalService::DoCloseChatAutoSaveCard(QString userId, QString vid)
{
    Q_EMIT S_CloseChatAutoSaveCard(userId, vid);
}


void PlatCommonSignalService::DoSaveVisitorCard(bool isNew)
{
    Q_EMIT S_SaveVisitorCard(isNew);
}

void PlatCommonSignalService::DoUpdate_package_download_completed()
{
    Q_EMIT S_Update_package_download_completed();
}

void PlatCommonSignalService::DoInitTopMenu()
{
    Q_EMIT S_InitTopMenu();
}

void PlatCommonSignalService::DoOpenAIWindow(QString key, QString jsonval)
{
    Q_EMIT S_OpenAIWindow(key, jsonval);
}

void PlatCommonSignalService::DoPopPersonWebWindow(QString key, QString jsonval, QString url)
{
    Q_EMIT S_PopPersonWebWindow(key, jsonval, url);
}

void PlatCommonSignalService::DoReturnPersonSetData(QString key, QString jsonval)
{
    Q_EMIT S_ReturnPersonSetData(key, jsonval);
}

void PlatCommonSignalService::DoReturnAIData(QString key, QString jsonval)
{
    Q_EMIT S_ReturnAIData(key, jsonval);
}
void PlatCommonSignalService::DoShowAiTab(int type, QString val)
{
    Q_EMIT S_ShowAiTab(type, val);
}

void PlatCommonSignalService::DoSetAiPanelScale()
{
    Q_EMIT S_SetAiPanelScale();
}

void PlatCommonSignalService::DoNotifyRobotNoReply(QString chatId, QString recordId)
{
    Q_EMIT S_NotifyRobotNoReply(chatId, recordId);
}

void PlatCommonSignalService::DoShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos)
{

    Q_EMIT S_ShowToastMsg(msg, showTime, showPos, type, tarPos);
}


void PlatCommonSignalService::DoShowToastMsg(QString msg, EYLUI_ToastType type)
{
    Q_EMIT S_ShowToastMsg(msg, type);
}

void PlatCommonSignalService::DoMainSelectChat(QString vid)
{
    Q_EMIT S_MainSelectChat(vid);
}

void PlatCommonSignalService::DoUpdateNoReadNum(MChatItemData *objData, int nums)
{
    Q_EMIT S_UpdateNoReadNum(objData, nums);
}

void PlatCommonSignalService::DoGetCurrentChatRecord(QString chatID)
{
    Q_EMIT S_GetCurrentChatRecord(chatID);
}
void PlatCommonSignalService::DoSendRobotStopByChatID(QString chatID)
{
    Q_EMIT S_SendRobotStopByChatID( chatID );
}

void PlatCommonSignalService::DoSaveHistoryMessage(QString body, QString chatid, QString from, EMESSAGETYPE type)
{
    Q_EMIT S_SaveHistoryMessage( body,  chatid,  from,  type);
}

void PlatCommonSignalService::DoSendMessageToAi(const Im_Message &msg)
{
    Q_EMIT S_SendMessageToAi(msg);
}


void PlatCommonSignalService::DoSendReceiptToAi(const QMap<QString, QString> &map)
{
    Q_EMIT S_SendReceiptToAi(map);
}

void PlatCommonSignalService::DoRecord_CreateTempChat(UIChatInfo* pInfo)
{
    Q_EMIT S_Record_CreateTempChat(pInfo);
}

void PlatCommonSignalService::DoAddSystemInfo(QString vid, QString chatId, QString msg)
{
    Q_EMIT S_AddSystemInfo(vid, chatId, msg);
}

void PlatCommonSignalService::DoTakeoverStatus(QString chatId, CHAT_TAKEOVER_FLAG flag)
{
    Q_EMIT S_TakeoverStatus(chatId, flag);
}


void PlatCommonSignalService::DoUpdateThirdCardTab(MUICardInfoData *data)
{
    Q_EMIT S_UpdateThirdCardTab(data);
}


void PlatCommonSignalService::DoOpenPicture(QString url)
{
    Q_EMIT S_OpenPicture(url);
}

