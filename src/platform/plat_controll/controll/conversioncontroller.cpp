#include "stdafx.h"
#include "ConversionController.h"

#include "protocol/platprotocolservice.h"
#include "service/PlatCommonSignalService.h"
#include "utils/ConstStringHelper.h"
#include "utils/DefineHelper.h"


#include "model/UIChatTreeModel.h"
#include "model/UIContactTreeModel.h"
#include "session/el_sessionservice.h"
#include "data/UIChatInfo.h"
#include "utils/utilshelper.h"

#include "service/util_service.h"
#include <service/platcommonmodelservice.h>

ConversionController::ConversionController(void)
{
}

ConversionController::~ConversionController(void)
{
}

void ConversionController::Init()
{
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnClickMonitorChatItem(MMonitorChatData *)), this, SLOT(R_OnClickMonitorChatItem(MMonitorChatData *)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnJYMainTreeMenuClick(QString, MChatItemData *, EUINODEMODELTYPE)), this, SLOT(R_OnJYMainTreeMenuClick(QString, MChatItemData *, EUINODEMODELTYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnClickChatItemData(MChatItemData *, EUINODEMODELTYPE)), this, SLOT(R_OnClickChatItemData(MChatItemData *, EUINODEMODELTYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnBeginDoubleClickChatItemData(MChatItemData *, EUINODEMODELTYPE)), this, SLOT(R_OnBeginDoubleClickChatItemData(MChatItemData *, EUINODEMODELTYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnContactSingleClick(MContactItemData *)), this, SLOT(R_OnContactSingleClick(MContactItemData *)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnContactDoubleClick(MContactItemData *)), this, SLOT(R_OnContactDoubleClick(MContactItemData *)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnCommonTreeMenuClick(QString, MContactItemData *)), this, SLOT(R_OnCommonTreeMenuClick(QString, MContactItemData *)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnChatItemCloseClicked(QString, QString)), this, SLOT(R_OnChatItemCloseClicked(QString, QString)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CloseChat(MChatItemData *)), this, SLOT(R_CloseChat(MChatItemData *)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_Inputting(MChatItemData *, QString)), this, SLOT(R_Inputting(MChatItemData *, QString)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_HiddenTransChat(MChatItemData *, QString)), this, SLOT(R_HiddenTransChat(MChatItemData *, QString)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnInviteEvaluationBtnClick(QString, QString)), this, SLOT(R_OnInviteEvaluationBtnClick(QString, QString)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnSendFile(EMESSAGETYPE , QString, EMESSAGETYPE)), this, SLOT(R_OnSendFile(EMESSAGETYPE , QString, EMESSAGETYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_DoShieldVisitor(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, int, const QString &, const QString &)), this,
             SLOT(R_DoShieldVisitor(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, int, const QString &, const QString &)));
     connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnTransferChat(const QString &, const QString &, const QString &, bool, const QString &)), this, SLOT(R_OnTransferChat(const QString &, const QString &, const QString &, bool, const QString &)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnClickButtonMonitorVisitorItem(QString, MMonitorVisitorData *)), this, SLOT(R_OnClickButtonMonitorVisitorItem(QString, MMonitorVisitorData *)));
}

void ConversionController::CreateChat(const UIChatInfo &info)
{
    if (!info.TempChat())
    {
        //对话扩展方法/////////////////////
        //gUIExtendConfigManager->ExtendPostCardOrderInfo(info.ChatId());
        //对话扩展方法/////////////////////
    }
}

void ConversionController::CreateContactChat(const UIContactInfo &info)
{
    UIChatTreeModel::GetInstance()->NotifyAddChatItem(info);
}

void ConversionController::CreateMonitorChat(const UIChatInfo &info)
{
    UIChatTreeModel::GetInstance()->NotifyAddChatItem(info, "monitor");
}

void ConversionController::DoCloseChat(QString chatid, QString vid)
{
}

void ConversionController::SendComposingToVisitor(const QString &chatId)
{
    if (chatId.isEmpty())
    {
        return;
    }
    gIProtocolAgent->DoExecCommChatMessageCmd(chatId, E_ACTION_GETFOCUS);
}

void ConversionController::DoSendInvite(const QString &userid, const QString &vid)
{
    //上报访客信息 todo
      UIVisitorUploadDataInfo *info = gSessionService->CreateUploadData();
      info->VisitorStaticId(userid);
      info->SecType("2001");
      info->ThirdType("2001001");
      UIPlatCommonDataModel::Instance()->UpdateVisitorUpdateData(info);
      gPlatProtocolService->DoVisitorUploadData(info);

    gIProtocolAgent->DoExecConversationCmd(userid, vid, "invite");
        delete info;
}

void ConversionController::DoSendInviteText(const QString &userid, const QString &vid, const QString &text)
{
    //上报访客信息
       UIVisitorUploadDataInfo *info = gSessionService->CreateUploadData();
       info->VisitorStaticId(userid);
       info->SecType("2001");
       info->ThirdType("2001003");
       UIPlatCommonDataModel::GetInstance()->UpdateVisitorUpdateData(info);
       gPlatProtocolService->DoVisitorUploadData(info);

    gIProtocolAgent->DoExecInviteText(userid, vid, text);
        delete info;
}

void ConversionController::DoSendForceInvite(const QString &userid, const QString &vid)
{
    //上报访客信息
       UIVisitorUploadDataInfo *info = gSessionService->CreateUploadData();
       info->VisitorStaticId(userid);
       info->SecType("2001");
       info->ThirdType("2001002");
       UIPlatCommonDataModel::GetInstance()->UpdateVisitorUpdateData(info);
       gPlatProtocolService->DoVisitorUploadData(info);

    gIProtocolAgent->DoExecConversationCmd(userid, vid, "forceInvite");
        delete info;
}

void ConversionController::DoSendAcceptActionById(QString vid)
{
    MContactItemData *objData = UIContactTreeModel::GetInstance()->GetContactItemNodeById(vid, E_UI_MODEL_QUEUE_VISITOR);
    DoSendAcceptAction(objData);
}


void ConversionController::DoQuitChatByChatId(QString chatid, bool tempDeleteCache)
{
    gIProtocolAgent->DoExecCommChatMessageCmd(chatid, E_Action_QUIT);
    gUIConversationManager->RemoveChatInfoByChatId(chatid, tempDeleteCache);
    gUIChatServiceManager->DeleteChatByChatId(chatid, 0);


}


void ConversionController::SendInviteEvaluationCmd(const QString &chatId)
{
    gIProtocolAgent->DoExecCommChatMessageCmd(chatId, E_Action_OPINION);
}

void ConversionController::DoSaveSummary(const QString &type, const QString &summary, const QString &chatId)
{
    gIProtocolAgent->DoSaveSummary(type, summary, chatId);
}

void ConversionController::DoTransChat(const QString &vid, const QString &userid, const QString &chatId, bool hasHistory, const QString &summary)
{
    CHAT_TAKEOVER_FLAG takeoverflag;
      if (gSessionService->UserInfo()->UserId() == userid)
          takeoverflag = CHAT_TAKEOVER_WHIRL;
      else
          takeoverflag = CHAT_TAKEOVER_TRANS;
      PlatCommonSignalService::Instance()->DoTakeoverStatus(chatId, takeoverflag);

      gPlatProtocolService->DoExecTakeoverChatCmd(chatId, "0", userid, 0);
      //	DoHiddenTransChat(vid, userid, chatId, hasHistory, summary, false);

      UIVisitorUploadDataInfo *info = gSessionService->CreateUploadData();
      info->ChatId(chatId);
      info->SecType("2001");
      info->ThirdType("2001003");
      UIPlatCommonDataModel::GetInstance()->UpdateVisitorUpdateData(info);
      gPlatProtocolService->DoVisitorUploadData(info);
      if (gSessionService->UserInfo()->UserId() == userid)
      {
          //抢接
          info->SecType("2009");
          info->ThirdType("2009002");
          //转移前对话id Long shiftFrontChatId、转移后对话id Long shiftLaterChatId、转移前客服id String shiftFrontUserId、转移后客服id String shiftLaterUserId
      }
      else
      {
          //转
          info->SecType("2008");
          info->ThirdType("2008002");
      }

      info->AddExtsMap("shiftFrontChatId", chatId);
      info->AddExtsMap("shiftLaterChatId", chatId);
      UIChatInfo *chatinfo = UICommonDataModel::GetInstance()->GetChatByChatId(chatId);
      if (chatinfo)
          info->AddExtsMap("shiftFrontUserId", chatinfo->CustomerID());

      info->AddExtsMap("shiftLaterUserId", userid);
      gPlatProtocolService->DoVisitorUploadData(info);
      info->deleteLater();
}

void ConversionController::DoHiddenTransChat(const QString &vid, const QString &userid, const QString &strChatId, bool hasHistory, const QString &summary, bool hidden)
{
    QString chatid = strChatId;
    if (chatid.isEmpty())
    {
        chatid = gUIConversationManager->GetChatIdByViewId(vid);
    }
    UIChatInfo *info = UICommonDataModel::GetInstance()->GetChatByChatId(chatid, 1);
    if (info == nullptr)
    {
        Q_ASSERT(false);
        return;
    }
    QString customerUserId = info->CustomerID();
    if (!chatid.isEmpty())
    {
        QString format = "visitorId=%1&u=%2&toId=%3&chatId=%4&msg=%5&hidden=%6";

        Json::Value            arrVal;
        int                    i    = 0;
        QList<UIHistoryInfo *> list = UICommonDataModel::GetInstance()->GetHistoryListByChatId(chatid);

        foreach (UIHistoryInfo *msg, list)
        {
            if (msg)
            {
                Json::Value val;
                val["from"]   = msg->From().toStdString();
                val["chatid"] = msg->ChatId().toStdString();
                if (msg->Type() == E_MSGTYPE_MESSAGE)
                {
                    val["type"] = "message";
                }
                else if (msg->Type() == E_MSGTYPE_FILE)
                {
                    val["type"] = "file";
                }
                else if (msg->Type() == E_MSGTYPE_SCREENSHOTS)
                {
                    val["type"] = "screenShots";
                }
                else if (msg->Type() == E_MSGTYPE_VIDEO)
                             {
                                 val["type"] = "video";
                             }
                             else if (msg->Type() == E_MSGTYPE_VOICE)
                             {
                                 val["type"] = "voice";
                             }
                             else if (msg->Type() == E_MSGTYPE_IMAGE)
                             {
                                 val["type"] = "image";
                             }
                             else if (msg->Type() == E_MSGTYPE_WXVOICE)
                             {
                                 val["type"] = "wxVoice";
                             }
                if (msg->Type() == E_MSGTYPE_MESSAGE && msg->Body().isEmpty())
                {
                    continue;
                }
                else
                {
                    val["body"] = msg->Body().toStdString();
                    val["time"] = msg->Time().toString("yyyy-MM-dd hh:mm:ss").toStdString();
                    arrVal[i]   = val;
                    i++;
                }
            }
        }
        UICommonDataModel::GetInstance()->ClearHistoryList(list);
        std::string out       = arrVal.toStyledString();
        QString     msgString = QString::fromStdString(out);
        QString     valHex    = msgString.toUtf8().toHex();

        QString ret = "";
        if (hidden)
        {
            ret = format.arg(vid).arg(customerUserId).arg(userid).arg(chatid).arg(valHex).arg("true");
        }
        else
        {
            ret = format.arg(vid).arg(customerUserId).arg(userid).arg(chatid).arg(valHex).arg("false");
        }
        gIProtocolAgent->DoTransChat(ret);
        //UIMainController::GetInstance()->RemoveChatItem(vid, IUIController::GetInstance()->GetCurrentUserId(), E_VSTATUS_CONVERSATION);
                //todo shizhenyu ，这个转移对话，先不立马删除底层对话。等待底层的ParseCloseChatMessage 在删除对话。
                //gUIConversationManager->RemoveChatInfoByChatId(chatid);
      qDeleteAll(list);
    }
    else
    {
        Q_ASSERT(false);
    }
}
void ConversionController::DoRevokeMessage(const QString &chatId, QString recordId)
{
    gPlatProtocolService->DoExecSendMsgCmd("", "", chatId, E_MSGTYPE_REVOKE, E_VISITOR, recordId, 0);
}


void ConversionController::DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map)
{
    gPlatProtocolService->DoExecSendAiControlCmd(chatid, map);
}



void ConversionController::UploadFile(const QString &fileName, const QString &chatId, EMESSAGETYPE msgType, EUSERTYPE type, bool advance)
{
    QString fileType = "file";
    if (msgType == E_MSGTYPE_FILE)
    {
        fileType = "file";
    }
    else if (msgType == E_MSGTYPE_SCREENSHOTS)
    {
        fileType = "screenShots";
    }
    else if (msgType == E_MSGTYPE_VIDEO)
        {
            fileType = "video";
        }
        else if (msgType == E_MSGTYPE_VOICE)
        {
            fileType = "voice";
        }
        else if (msgType == E_MSGTYPE_IMAGE)
        {
            fileType = "image";
        }
    else
    {
        Q_ASSERT(false);
        GERRORLOG("未知的文件上传类型！！！");
        return;
    }
    if (!advance)
    {
        fileType = "noadvancenotice"; //不需要提前通知web发送了文件
    }
    QString uploadUrl = "";
    //todo 9999 改用system
    /*
       if (type != E_USERTYPE_DEFAULT)
       {
           uploadUrl = gUIPersonalConfigManager->FileUploadURL();
           if (uploadUrl.isEmpty())
           {
               uploadUrl = gSessionService->GetGlobalConfig()->GetValueByKey("fileUploadURL");
           }
       }
       else
       {
           fileType  = "client";
           uploadUrl = gUIPersonalConfigManager->ExpressionUploadUrl(); //表情上传地址
       }
    gIProtocolAgent->DoUploadFile(uploadUrl, fileName, fileType, chatId,  gSessionService->UserInfo()->CompanyId(), type); //发送文件开始 5
    */
}

//todo 9999
/*

void ConversionController::UploadFile(MChatItemData *objData, QString fileName, EMESSAGETYPE msgType)
{
    if (objData)
    {
        if (objData->UserType() == E_VISITOR)
        {
            UploadFile(fileName, objData->Chatid(), msgType, objData->UserType()); //发送文件开始 4
        }
        else
        {
            UploadFile(fileName, objData->Chatid(), msgType, objData->UserType());
        }
    }
}

void ConversionController::DoShieldVisitor(const QString &id, const QString &ip, const QString &type, const QString &expires, const QString &note, const QString &chatId, int flag, const QString &scope, const QString &groupId)
{
    gIProtocolAgent->DoShieldVisitor(id, ip, type, expires, note, scope, groupId);
     if (flag == 1)
    {
        gIProtocolAgent->DoExecCommChatMessageCmd(chatId, E_Action_QUIT);
        QString viewId = gUIConversationManager->GetViewIdByChatId(chatId);
        PlatCommonSignalService::Instance()->DoReceiveChatCloseMsg(chatId, viewId, "close");
        gUIConversationManager->RemoveChatInfoByChatId(chatId);
    }
    if (type == "2")
    {
        //通过ipv6屏蔽
        // zcmadd ipv6屏蔽
    }
}
*/


void ConversionController::SendQuitSameVisitorChatMessageCmd(const QString &chatid, const QString &vid)
{
    gIProtocolAgent->DoExecCommChatMessageCmd(chatid, E_ACTION_QUITSAMEVISITOR, vid);
    gUIConversationManager->RemoveChatInfoByChatId(chatid, true);
}

void ConversionController::R_OnJYMainTreeMenuClick(QString type, MChatItemData *item, EUINODEMODELTYPE nodeType)
{
    if (StringCompare(type, ConstStringHelper::m_cMonitorVisitorChat))
    {
        if (item)
        {
            if (nodeType == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
            {
                //监控对话
                CommonModelService::GetInstance()->DoSendMonitorChatById(item->Chatid());
            }
            else if (nodeType == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE)
            {
                QList<MContactItemData *> list = UIContactTreeModel::GetInstance()->GetContactItemNodeByCustomerId(item->Vid());
                Q_FOREACH (MContactItemData *itemObj, list)
                {
                    CommonModelService::GetInstance()->DoSendMonitorChatById(itemObj->Chatid());
                }
            }
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cInnerContactChat))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cInviteChat))
    {
        if (item)
        {
            DoSendInvite(item->Userid(), item->Vid());
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cDirectChat))
    {
        if (item)
        {
            DoSendForceInvite(item->Userid(), item->Vid());
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cShieldVisitor))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cEditCard))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cSummary))
    {
    }
    else if (StringCompare(type, "clearall_leave_visitor"))
    {
    }
    else
    {
        Q_ASSERT(false);
    }
}

void ConversionController::R_OnClickChatItemData(MChatItemData *obj, EUINODEMODELTYPE noteType)
{
    if (obj)
    {
        if (noteType == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR && obj->GetExtendData("CustomerID") != gSessionService->UserInfo()->UserId())
        {
            //监控对话
            CommonModelService::GetInstance()->DoSendMonitorChatById(obj->Chatid());
        }
    }
}

void ConversionController::R_OnBeginDoubleClickChatItemData(MChatItemData *item, EUINODEMODELTYPE noteType)
{
    if (item == nullptr)
    {
        return;
    }

    switch (noteType)
    {
        case E_UI_MODEL_QUEUE_VISITOR:
            gIProtocolAgent->DoExecSendAcceptActionCmd(item->GetExtendData("GroupId"), item->Vid());
            break;
        case E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR:
            //监控对话
            if (noteType == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR && item->GetExtendData("CustomerID") != gSessionService->UserInfo()->UserId())
            {
                CommonModelService::GetInstance()->DoSendMonitorChatById(item->Chatid());
            }
            break;
        case E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR:
        {
            EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
            if (state == E_VSTATUS_BROWSE_WEB)
            {
                DoSendInvite(item->Userid(), item->Vid());
            }
        }
        break;
        default:
            break;
    }
}

void ConversionController::R_OnContactSingleClick(MContactItemData *item)
{
    if (item)
    {
        if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR && item->GetExtendData("CustomerID") != gSessionService->UserInfo()->UserId())
        {
            //监控对话
            CommonModelService::GetInstance()->DoSendMonitorChatById(item->Chatid());
        }
    }
}

void ConversionController::R_OnContactDoubleClick(MContactItemData *item)
{
    if (item == nullptr)
    {
        return;
    }

    switch (item->NoteType())
    {
        case E_UI_MODEL_QUEUE_VISITOR:
            DoSendAcceptAction(item);
            break;
        case E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR:
            //监控对话
            if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR && item->GetExtendData("CustomerID") != gSessionService->UserInfo()->UserId())
            {
                CommonModelService::GetInstance()->DoSendMonitorChatById(item->Chatid());
            }
            break;
        case E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR:
        {
            EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
            if (state == E_VSTATUS_BROWSE_WEB)
            {
                DoSendInvite(item->Userid(), item->Vid());
            }
        }
        break;
        default:
            break;
    }
}

void ConversionController::R_OnCommonTreeMenuClick(QString type, MContactItemData *item)
{
    if (StringCompare(type, ConstStringHelper::m_cMonitorVisitorChat))
    {
        if (item)
        {
            if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
            {
                //监控对话
                CommonModelService::GetInstance()->DoSendMonitorChatById(item->Chatid());
            }
            else if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE)
            {
                QList<MContactItemData *> list = UIContactTreeModel::GetInstance()->GetContactItemNodeByCustomerId(item->GetUUid());
                Q_FOREACH (MContactItemData *itemObj, list)
                {
                    CommonModelService::GetInstance()->DoSendMonitorChatById(itemObj->Chatid());
                }
            }
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cInnerContactChat))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cInviteChat))
    {
        if (item)
        {
            DoSendInvite(item->Userid(), item->Vid());
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cDirectChat))
    {
        if (item)
        {
            DoSendForceInvite(item->Userid(), item->Vid());
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cShieldVisitor))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cEditCard))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cSummary))
    {
    }
    else
    {
        Q_ASSERT(false);
    }
}

void ConversionController::DoSendAcceptAction(MContactItemData *objData)
{
    if (objData)
    {
        gIProtocolAgent->DoExecSendAcceptActionCmd(objData->GetExtendData("GroupId"), objData->GetUUid());
    }
}

void ConversionController::R_CloseChat(MChatItemData *dataObj)
{
    if (dataObj)
    {
        //关闭对话
        ConversionController::GetInstance()->DoQuitChatByChatId(dataObj->Chatid(), true);
        gIProtocolAgent->DoUploadOperatorLog("online_client_conversation_29");
    }
}

void ConversionController::R_Inputting(MChatItemData *objData, QString message)
{
    if (objData)
    {
        SendComposingToVisitor(objData->Chatid());
    }
}

void ConversionController::R_HiddenTransChat(MChatItemData *objData, QString userid)
{
    if (objData)
    {
        DoHiddenTransChat(objData->Vid(), userid, objData->Chatid(), false, "", true);
    }
}

void ConversionController::R_OnChatItemCloseClicked(QString vid, QString cid)
{
    if (!cid.isEmpty())
    {
        DoQuitChatByChatId(cid, false);
    }
    else
    {
        UIChatInfo *info = UICommonDataModel::GetInstance()->GetChatByVid(vid);
        if (info)
        {
            DoQuitChatByChatId(info->ChatId(), false);
        }
    }
}

void ConversionController::R_OnInviteEvaluationBtnClick(QString chatId, QString vid)
{
    SendInviteEvaluationCmd(chatId);
}

void ConversionController::R_OnSendFile(MChatItemData *dataObj, QString fileName, EMESSAGETYPE msgType)
{
    UploadFile(dataObj, fileName, msgType); //发送文件开始 3
}

void ConversionController::R_DoShieldVisitor(const QString &id, const QString &ip, const QString &type, const QString &expires, const QString &note, const QString &chatId, int flag, const QString &scope, const QString &groupId)
{
    //todo 9999
    //DoShieldVisitor(id, ip, type, expires, note, chatId, flag, scope, groupId);
}

void ConversionController::R_OnTransferChat(const QString &vid, const QString &userid, const QString &chatId, bool hasHistory, const QString &summary)
{
    DoTransChat(vid, userid, chatId, hasHistory, summary);
}

void ConversionController::R_OnClickMonitorChatItem(MMonitorChatData *item)
{
    if (item)
    {
        if (item->GetUIChatInfoValueByKey("customerId") != gSessionService->UserInfo()->UserId())
        {
            //监控对话
            CommonModelService::GetInstance()->DoSendMonitorChatById(item->ChatId());
        }
    }
}

void ConversionController::R_OnClickButtonMonitorVisitorItem(QString type, MMonitorVisitorData *objData)
{
    if (type == tr("直接对话") || type == "direct")
    {
        if (objData)
        {
            DoSendForceInvite(objData->Userid(), objData->Vid());
        }
    }
    else if (type == tr("邀请对话") || type == "invite")
    {
        if (objData)
        {
            DoSendInvite(objData->Userid(), objData->Vid());
        }
    }
}

void ConversionController::DoRandomAcceptAction()
{
    gIProtocolAgent->DoExecRandomSendAcceptActionCmd();
}


void ConversionController::DoAllocationChat(const QString &vid, const QString &groupId, const QString &userid)
{
    gIProtocolAgent->DoAllocationChat(vid, groupId, userid);
}

void ConversionController::DoTransChatFormOtherToOther(const QString &vid, const QString &chatid, const QString &fromUserid, const QString &toUserId)
{
    if (!chatid.isEmpty())
    {
        QString                format = "visitorId=%1&u=%2&toId=%3&chatId=%4&msg=%5&hidden=%6";
        Json::Value            arrVal;
        int                    i    = 0;
        QList<UIHistoryInfo *> list = UICommonDataModel::GetInstance()->GetHistoryListByChatId(chatid);
        foreach (UIHistoryInfo *msg, list)
        {
            if (msg)
            {
                Json::Value val;
                val["from"]   = msg->From().toStdString();
                val["chatid"] = msg->ChatId().toStdString();
                if (msg->Type() == E_MSGTYPE_MESSAGE)
                {
                    val["type"] = "message";
                }
                else if (msg->Type() == E_MSGTYPE_FILE)
                {
                    val["type"] = "file";
                }
                else if (msg->Type() == E_MSGTYPE_SCREENSHOTS)
                {
                    val["type"] = "screenShots";
                }
                              else if (msg->Type() == E_MSGTYPE_VIDEO)
                              {
                                  val["type"] = "video";
                              }
                              else if (msg->Type() == E_MSGTYPE_VOICE)
                              {
                                  val["type"] = "voice";
                              }
                              else if (msg->Type() == E_MSGTYPE_IMAGE)
                              {
                                  val["type"] = "image";
                              }
                              else if (msg->Type() == E_MSGTYPE_WXVOICE)
                              {
                                  val["type"] = "wxVoice";
                }
                else
                {
                    // do noting
                }
                if (msg->Type() == E_MSGTYPE_MESSAGE && msg->Body().isEmpty())
                {
                    continue;
                }
                else
                {
                    val["body"] = msg->Body().toStdString();
                    val["time"] = msg->Time().toString("yyyy-MM-dd hh:mm:ss").toStdString();
                    arrVal[i]   = val;
                    i++;
                }
            }
            UICommonDataModel::GetInstance()->ClearHistoryList(list);
            qDeleteAll(list);
        }

        std::string out       = arrVal.toStyledString();
        QString     msgString = QString::fromStdString(out);
        QString     valHex    = msgString.toUtf8().toHex();
        QString     ret       = format.arg(vid).arg(fromUserid).arg(toUserId).arg(chatid).arg(valHex).arg("true");
        gIProtocolAgent->DoTransChat(ret);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void ConversionController::UploadFile(MChatItemData *objData, QString fileName, EMESSAGETYPE msgType)
{
    //todo 9999

}
