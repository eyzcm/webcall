#include "stdafx.h"
#include "PlatCommonModelService.h"
/*
#include "control/CommonSignalService.h"
#include "control/PlatCommonModelService.h"
#include "service/DbUiWebcallService.h"
#include "protocol_biz/webcallprotocolservice.h"
*/
#include "session/el_sessionservice.h"
#include <model/uiplatcommondatamodel.h>
#include <utils/utilshelper.h>
#include "utils/jsonhelper.h"

#include "session/el_sessionservice.h"
#include "data/UIVisitorUploadDataInfo.h"
#include "protocol/platprotocolservice.h"
#include "data/UIChatInfo.h"
#include "serviceinterface/dbuiplatservice.h"


PlatCommonModelService::PlatCommonModelService(QObject *parent) : QObject(parent)
{
    m_clientType = E_CLIENT_NEWMEDICAL;
}

PlatCommonModelService::~PlatCommonModelService(void)
{

}


UISystemTipData *PlatCommonModelService::GetSystemTipByKey(QString type)
{
    return UIPlatCommonDataModel::Instance()->GetSystemTipByKey(type);
}


//todo 9999
/*

QString PlatCommonModelService::GetSystemConfigByKey(const QString &key, const QString &defaultString)
{
    QString val = gUIPersonalConfigManager->GetOtherConfigByKey(key, "");
    if (val.isEmpty())
    {
        val = gUISystemConfigManager->GetSystemConfigByKey(key, defaultString);
    }
    if (key == "webpath")
       {
           QString htmlenv     = gUIPersonalConfigManager->HtmlEnv();
           QString HtmlVersion = gUIPersonalConfigManager->HtmlVersion();
           if (!htmlenv.isEmpty() && !HtmlVersion.isEmpty())
           {
               val = val.replace("/group-prd1/", QString("/%1/").arg(htmlenv));
               val = val.replace("/defaultVersion/", QString("/%1/").arg(HtmlVersion));
           }
           // js页面增加版本控制
       }
    return val;
}
*/

void PlatCommonModelService::SendFileMsg(const QString &fileName, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, int callbackId, QMap<QString, QString> map)
{
    gPlatProtocolService->DoExecSendMsgCmd("", fileName, msgTo, msgType, userType, "", callbackId, map);
}


void PlatCommonModelService::DoTransChatForMe(const QString &vid, const QString &chatid, const QString &fromUserid)
{
    if (!chatid.isEmpty())
      {
          gCommonSignalService->DoTakeoverStatus(chatid, CHAT_TAKEOVER_WHIRL);
          UIVisitorUploadDataInfo *info = gSessionService->CreateUploadData();
          info->ChatId(chatid);
          UIPlatCommonDataModel::Instance()->UpdateVisitorUpdateData(info);
          //抢接
          info->SecType("2009");
          info->ThirdType("2009002");
          /*
          转移前对话id Long shiftFrontChatId、转移后对话id Long shiftLaterChatId、转移前客服id String shiftFrontUserId、转移后客服id String shiftLaterUserId
          */
          info->AddExtsMap("shiftFrontChatId", chatid);
          info->AddExtsMap("shiftLaterChatId", chatid);
          info->AddExtsMap("shiftFrontUserId", fromUserid);
          info->AddExtsMap("shiftLaterUserId", gSessionService->UserInfo()->UserId());
          gPlatProtocolService->DoVisitorUploadData(info);
          gPlatProtocolService->DoExecTakeoverChatCmd(chatid, "", "", CHAT_TAKEOVER_WHIRL);
		  delete info;
      }
      else
      {
          Q_ASSERT(false);
      }
}

ECLIENTTYPE  PlatCommonModelService::GetClientType() const
{
    return m_clientType;
}


void PlatCommonModelService::DoSendMonitorChatById(const QString &id)
{
    gPlatProtocolService->DoExecSendMonitorChatCmd(id);
}


//ConversionController移槙过来
void PlatCommonModelService::SendRichTextMsg(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, CommInfo aiOtherInfo, QString actionId, int callbackId, QMap<QString, QString> map)
{
    gPlatProtocolService->DoExecSendRichTextMsgCmdEx(msgBody, msgTo, msgType, userType, recordId, aiOtherInfo, actionId, callbackId, map); //发送文字消息 6
        gCommonSignalService->DoSaveHistoryMessage(msgBody, msgTo, gSessionService->UserInfo()->UserId(), msgType);
       if ((actionId.isEmpty()) || (actionId == "0"))
       {
           if ((msgType == E_MSGTYPE_MESSAGE) || (msgType == E_MSGTYPE_SCREENSHOTS) || (msgType == E_MSGTYPE_VIDEO) || (msgType == E_MSGTYPE_VOICE) || (msgType == E_MSGTYPE_IMAGE) || (msgType == E_MSGTYPE_FILE))
           {
               QString isAutoJoin = gSessionService->GetSystemConfigByKey("autojoinchat", "0");
               if (isAutoJoin == "1")
               {
                   UIChatInfo *info = UIPlatCommonDataModel::Instance()->GetChatByChatId(msgTo);
                   if ((info) && (info->CustomerID() == gSessionService->UserInfo()->UserId()))
                   {
                       gCommonSignalService->DoAutoJoinTagBySendMsg(msgTo);
                   }
               }
           }
       }
       do
       {
           UIChatInfo *info = UIPlatCommonDataModel::Instance()->GetChatByChatId(msgTo);
           if ((info) && (info->CustomerID() == gSessionService->UserInfo()->UserId()))
           {
               //继续
           }
           else
           {
               //不发送
               break;
           }
           QString isAutoReply =  gSessionService->GetSystemConfigByKey("isautoreply", "0");
           if (isAutoReply == "0")
           {
               // do nothing
           }
           else
           {
               // //开启了机器人
               // bool isRobot = UtilsHelper::IsRobot(gSessionService->UserInfo()->AiFlag(), gSessionService->UserInfo()->UserId());
               // if (isRobot && actionId.isEmpty())
               // {
               //     //如果是机器人，那么就不走绑定机器人。
               //     //客服消息
               //     gPlatProtocolService->DoSendUserMessageToRobot(gSessionService->UserInfo()->UserId(), msgTo, msgBody, recordId, map);
               //     break;
               // }
           }



       } while (false);
}


void PlatCommonModelService::SendMsg(const QString &msgBody, const QString &msgTo, EMESSAGETYPE msgType, EUSERTYPE userType, QString recordId, int callbackId, QMap<QString, QString> map)
{
    gPlatProtocolService->DoExecSendMsgCmd(msgBody, "", msgTo, msgType, userType, recordId, callbackId, map);

    gCommonSignalService->DoSaveHistoryMessage(msgBody, msgTo, gSessionService->UserInfo()->UserId(), msgType);
    if ((msgType == E_MSGTYPE_MESSAGE) || (msgType == E_MSGTYPE_SCREENSHOTS) || (msgType == E_MSGTYPE_VIDEO) || (msgType == E_MSGTYPE_VOICE) || (msgType == E_MSGTYPE_IMAGE) || (msgType == E_MSGTYPE_FILE))
    {
        QString isAutoJoin = gSessionService->GetSystemConfigByKey("autojoinchat", "0");
        if (isAutoJoin == "1")
        {
            UIChatInfo *info = UIPlatCommonDataModel::Instance()->GetChatByChatId(msgTo);
            if ((info) && (info->CustomerID() == gSessionService->UserInfo()->UserId()))
            {
                gCommonSignalService->DoAutoJoinTagBySendMsg(msgTo);
            }
        }
    }
    do
    {
        UIChatInfo *info = UIPlatCommonDataModel::Instance()->GetChatByChatId(msgTo);
        if ((info) && (info->CustomerID() == gSessionService->UserInfo()->UserId()))
        {
            //继续
        }
        else
        {
            //不发送
            break;
        }
        QString isAutoReply = gSessionService->GetSystemConfigByKey("isautoreply", "0");
        if (isAutoReply == "0")
        {
            // do nothing
        }
        else
        {
            //开启了机器人

        }


    } while (false);
}


void PlatCommonModelService::DoAutoQuitChatByChatId(QString chatId)
{
    gPlatProtocolService->DoExecCommChatMessageCmd(chatId, E_Action_SYS_QUIT);
    QString viewId = gUIConversationManager->GetViewIdByChatId(chatId);
    gUIConversationManager->RemoveChatInfoByChatId(chatId);
}


bool PlatCommonModelService::HasRecordHistoryByChatId(const QString chatid, const QString vid)
{
    bool bret = gUIHistoryManager->HasRecordHistoryByChatId(chatid, vid);
    if (!bret)
    {
        UIChatInfo *info = UIPlatCommonDataModel::Instance()->GetChatInfoByChatId(chatid);
        if (info)
        {
            if (info->VisitorMsgCount() != "0")
            {
                bret = true;
            }
        }
    }
    return bret;
}
void  PlatCommonModelService::R_LoginReturnData(QString body)
{
	Json::Reader reader;
	Json::Value  val;

	std::string bodyStr = body.toUtf8().data();

	if (reader.parse(bodyStr, val))
	{
#if 0
        QString mobileHiddenTime = JsonHelper::GetValueByKey("mobileHiddenTime", val);
        gUIPersonalConfigManager->MobileHiddenTime(mobileHiddenTime);

        QString mobileHidden = JsonHelper::GetValueByKey("mobileHidden", val);
        gUIPersonalConfigManager->MobileHidden(mobileHidden);



		// SummarizeStatus
		ESUMMARIZEONCLOSE eSummarizeStatus = E_FORCE;
		QString           SummarizeStatus = JsonHelper::GetValueByKey("summarizeOnClose", val);
		if (SummarizeStatus == "0")
		{
			eSummarizeStatus = E_NO_FORCE;
		}
		else if (SummarizeStatus == "1")
		{
			eSummarizeStatus = E_FORCE;
		}
		else if (SummarizeStatus == "2")
		{
			eSummarizeStatus = E_TIP;
		}
		gUIPersonalConfigManager->SummarizeStatus(eSummarizeStatus);

		QString exts = JsonHelper::GetValueByKey("exts", val);
		gUIPersonalConfigManager->SetExtString(exts);
#endif
	}

}
