#include "UIBaseFactory.h"
#include "utils/ConstDefine.h"
#include "utils/ConstStringHelper.h"
#include "serviceinterface/dbuiplatservice.h"
#include "session/el_sessionservice.h"
#include "serviceinterface/dbuiplatservice.h"

UIBaseFactory::UIBaseFactory(void)
{
}

UIBaseFactory::~UIBaseFactory(void)
{
}

void* UIBaseFactory::CreateObj(const QString& datatype, void* itemobj)
{
    void* ret = nullptr;

    ret = ConstructionData::CreateObj(datatype, itemobj);
    do
    {
        if (ret != nullptr)
        {
            break;
        }
        if (IsIndexOfString(datatype, "E_VISITOR_WAIT"))
        {
            ret = BuilderChatItemData((UIWaitVisitorInfo*)itemobj);
        }
        else if (IsIndexOfString(datatype, "E_VISITOR_MONITOR"))
        {
            ret = BuilderChatItemData((UIVisitorInfo*)itemobj);
        }
        else if (IsIndexOfString(datatype, "E_CHATITEM_CHAT"))
        {
            ret = BuilderChatItemData((UIChatInfo*)itemobj, E_CHATITEM_CHAT);
        }
        else if (IsIndexOfString(datatype, "E_CHATITEM_CHAT_CLOSE"))
        {
            ret = BuilderChatItemData((UIChatInfo*)itemobj, E_CHATITEM_CHAT_CLOSE);
        }
        else if (IsIndexOfString(datatype, "E_CHATITEM_INVITE"))
        {
            ret = BuilderChatItemData((UIChatInfo*)itemobj, E_CHATITEM_INVITE);
        }
        else if (IsIndexOfString(datatype, "E_CHATITEM_MONITOR_CHAT"))
        {
            ret = BuilderChatItemData((UIChatInfo*)itemobj, E_CHATITEM_MONITOR_CHAT);
        }
        else if (IsIndexOfString(datatype, "E_CHATITEM_MONITOR_CHAT_CLOSE"))
        {
            ret = BuilderChatItemData((UIChatInfo*)itemobj, E_CHATITEM_MONITOR_CHAT_CLOSE);
        }
        else if (IsIndexOfString(datatype, "E_UI_CHAT_CUSTOMER"))
        {
            ret = BuilderChatItemData((UIContactInfo*)itemobj);
        }
        else if (IsIndexOfString(datatype, "E_CHATITEM_ALL_CACHE_CHAT"))
        {
            ret = BuilderChatItemData((UIChatInfo*)itemobj);
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_QUEUE_VISITOR"))
        {
            ret = BuilderContactItemData(*(UIWaitVisitorInfo*)itemobj, E_UI_MODEL_QUEUE_VISITOR);
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_MONITOR_CHAT_COLLEAGUE"))
        {
            ret = BuilderContactItemData(*(UIContactInfo*)itemobj, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE);
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_MONITOR_CHAT_VISITOR"))
        {
            ret = BuilderContactItemData(*(UIChatInfo*)itemobj, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_COLLEAGUE_GROUP"))
        {
            ret = BuilderContactItemData(*(UIContactGroupInfo*)itemobj, E_UI_MODEL_COLLEAGUE_GROUP);
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_COLLEAGUE_CUSTOMER"))
        {
            ret = BuilderContactItemData(*(UIContactInfo*)itemobj, E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER);
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
        {
            ret = BuilderContactItemData(*(UIVisitorInfo*)itemobj, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
        {
            ret = BuilderContactItemData(*(UIVisitorInfo*)itemobj, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);

        }
        else
        {
             if ( (IsIndexOfString(datatype, "E_UI_MCARDCOLDATA"))
                  ||(IsIndexOfString(datatype, "E_UI_MCOMMONWORDDATA"))
                 )
                  break;
             else
                Q_ASSERT(false);
        }
    } while (false);

    return ret;
}

int UIBaseFactory::AddObjToList(const QString& datatype, void* itemobj, void* outlist)
{
    int ret = ConstructionData::AddObjToList(datatype, itemobj, outlist);
    do
    {
        if (ret > 0)
        {
            break;
        }
        if (IsIndexOfString(datatype, "E_UI_MODEL_COLLEAGUE_GROUP-GROUPMAP"))
        {

            MContactItemData* info = (MContactItemData*)itemobj;
            if (info)
            {
                QMap<QString, MContactItemData*>* pmap = (QMap<QString, MContactItemData*>*)outlist;
                pmap->insert(info->GetUUid(), info);
            }

            ret = 200;
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR_VISITORMAP"))
        {

            MContactItemData* info = (MContactItemData*)itemobj;
            if (info)
            {
                QMap<QString, MContactItemData*>* pmap = (QMap<QString, MContactItemData*>*)outlist;
                pmap->insert(info->GetUUid(), info);
            }

            ret = 200;
        }
        else if (IsIndexOfString(datatype, "E_UI_MODEL_QUEUE_VISITOR-QUEUEMAP"))
        {
            MContactItemData* info = (MContactItemData*)itemobj;
            if (info)
            {
                QMap<QString, MContactItemData*>* pmap = (QMap<QString, MContactItemData*>*)outlist;
                pmap->insert(info->GetUUid(), info);
            }

            ret = 200;
        }
        else if (IsIndexOfString(datatype, "E_CHATITEM_ALL_CACHE_CHAT-CHATMAP"))
        {
            MChatItemData* info = (MChatItemData*)itemobj;
            if (info)
            {
                QMap<QString, MChatItemData*>* pmap = (QMap<QString, MChatItemData*>*)outlist;
                pmap->insert(info->Vid(), info);
            }
            ret = 200;
        }
        else
        {
            Q_ASSERT(false);
        }

    } while (false);
    return ret;
}

MChatItemData* UIBaseFactory::BuilderChatItemData(UIWaitVisitorInfo* info)
{
    return CreateMChatItemData(*info);
}

MChatItemData* UIBaseFactory::BuilderChatItemData(UIContactInfo* info)
{
    return CreateMChatItemData(*info);
}

MChatItemData* UIBaseFactory::BuilderChatItemData(UIVisitorInfo* info)
{
    return CreateMChatItemData(*info);
}

MChatItemData* UIBaseFactory::BuilderChatItemData(UIChatInfo* info, ECHATITEMSTATE state)
{
    return CreateMChatItemData(*info, state);
}

MChatItemData* UIBaseFactory::BuilderChatItemData(UIChatInfo* info)
{
    return CreateMChatItemData(*info);
}

MContactItemData* UIBaseFactory::BuilderContactItemData(const UIWaitVisitorInfo& info, EUINODEMODELTYPE type)
{
    MContactItemData* ret = nullptr;

    QString strDisplayName = gUICacheService->GetDisplayName(&info);
    ret                    = new MContactItemData();
    ret->InitData(info.UserId(), info.UserId(), info.ViewVid(), "", strDisplayName, "", "", type);
    ret->AddExtendData("JoinQueueTime", info.JoinQueueTime());
    ret->AddExtendData("GroupId", info.GroupId());
    ret->AddExtendData("ip", info.Visitor_ip());
    ret->AddExtendData("ipv6", info.Visitor_ipv6());
    ret->AddExtendData("Device", info.Chat_type());
    if (info.CardInfo())
    {
        ret->AddExtendData("hasCard", "1");
        ret->AddExtendData("cardName", info.CardInfo()->Name());
        ret->AddExtendData("cardTel", info.CardInfo()->Tel());
        ret->AddExtendData("cardMobile", info.CardInfo()->Mobile());
        ret->AddExtendData("cardPhoneStatus", info.CardInfo()->PhoneStatus());
        ret->AddExtendData("cardNote", info.CardInfo()->Note());
        ret->AddExtendData("cardWX", info.CardInfo()->Msn());
        ret->AddExtendData("cardQQ", info.CardInfo()->Qq());
        if (info.CardInfo()->IsHasContact())
        {
            ret->AddExtendData("IsHasContact", "1");
        }
        else
        {
            ret->AddExtendData("IsHasContact", "0");
        }

        if (info.CardInfo()->IsAiEdit())
        {
            ret->AddExtendData("IsAiEdit", "1");
        }
        else
        {
            ret->AddExtendData("IsAiEdit", "0");
        }
    }
    else
    {
        ret->AddExtendData("hasCard", "0");
        ret->AddExtendData("IsHasContact", "0");
        ret->AddExtendData("IsAiEdit", "1");
    }

    return ret;
}

MContactItemData* UIBaseFactory::BuilderContactItemData(const UIContactInfo& info, EUINODEMODELTYPE type)
{
    MContactItemData* ret = nullptr;

    QString strDisplayName = info.DisplayInnerName();
    ret                    = new MContactItemData();
    ret->InitData(info.UserId(), info.UserId(), info.UserId(), info.UserId(), strDisplayName, "", info.DepartmentId(), type);
    ret->AddExtendData("Status", QString::number((int)info.Status()));
    ret->AddExtendData("loginType", QString::number((int)info.LoginType()));
    ret->AddExtendData("AiFlag", QString::number(info.AiFlag()));

    return ret;
}

MContactItemData* UIBaseFactory::BuilderContactItemData(const UIContactGroupInfo& info, EUINODEMODELTYPE type)
{
    MContactItemData* ret = nullptr;

    QString strDisplayName = info.DepartmentName();
    ret                    = new MContactItemData();
    ret->InitData(info.DepartmentId(), info.DepartmentId(), info.DepartmentId(), "", strDisplayName, "", info.DepartmentId(), type);

    return ret;
}

MContactItemData* UIBaseFactory::BuilderContactItemData(const UIVisitorInfo& info, EUINODEMODELTYPE type)
{
    QString strDisplayName = gUICacheService->GetDisplayName(&info);

    MContactItemData* ret = new MContactItemData();
    ret->InitData(info.Vid(), info.Vid(), info.StaticId(), "", strDisplayName, "", "", type);
    ret->SearchingType(info.SearchingType());
    ret->Awake(info.Awake());
    ret->AwakeInfo(info.AwakeInfo());
    ret->NickName(info.NickName());

    ret->AddExtendData("Device", info.Device());
    ret->AddExtendData("SearchEngineId", info.SearchEngineId());
    ret->AddExtendData("visitor_keyWord", info.KeyWord());
    ret->AddExtendData("visitor_location", info.LocalOption());
    ret->AddExtendData("visitor_Name", gUICacheService->GetDisplayName(&info));
    ret->AddExtendData("ip", info.Ip());
    ret->AddExtendData("visitor_hasCard", info.HasCard());
    ret->AddExtendData("visitorCount", info.VisitorCount());
    ret->AddExtendData(DOUYI_HEADIMGURL, info.HeadImgUrl());
    ret->AddExtendData(DOUYI_GENDER, info.Gender());


    if (info.CardInfo())
    {
        ret->AddExtendData("hasCard", "1");
        ret->AddExtendData("cardName", info.CardInfo()->Name());
        ret->AddExtendData("cardTel", info.CardInfo()->Tel());
        ret->AddExtendData("cardMobile", info.CardInfo()->Mobile());
        ret->AddExtendData("cardPhoneStatus", info.CardInfo()->PhoneStatus());
        ret->AddExtendData("cardNote", info.CardInfo()->Note());
        ret->AddExtendData("cardWX", info.CardInfo()->Msn());
        ret->AddExtendData("cardQQ", info.CardInfo()->Qq());
        if (info.CardInfo()->IsHasContact())
        {
            ret->AddExtendData("IsHasContact", "1");
        }
        else
        {
            ret->AddExtendData("IsHasContact", "0");
        }
        if (info.CardInfo()->IsAiEdit())
        {
            ret->AddExtendData("IsAiEdit", "1");
        }
        else
        {
            ret->AddExtendData("IsAiEdit", "0");
        }
    }
    else
    {
        ret->AddExtendData("hasCard", "0");
        ret->AddExtendData("IsHasContact", "0");
        ret->AddExtendData("IsAiEdit", "1");
    }
    return ret;
}

MContactItemData* UIBaseFactory::BuilderContactItemData(const UIChatInfo& info, EUINODEMODELTYPE type)
{
    MContactItemData* ret = nullptr;

    UIChatUserInfo* userInfo = info.GetVisitorInfo();
    if (userInfo)
    {
        ret = new MContactItemData();
        if (info.TempChat())
        {
            ret->InitData(userInfo->UserId(), userInfo->UserId(), userInfo->StaticId(), info.ChatId(), gUICacheService->GetDisplayName(userInfo->UserId()), "", "", type);
        }
        else
        {
            ret->InitData(userInfo->UserId(), userInfo->UserId(), userInfo->StaticId(), info.ChatId(), gUICacheService->GetDisplayName(userInfo), "", "", type);
        }
        ret->InviteMode(info.InviteMode());
        ret->SearchingType(info.SearchingType());
        ret->Awake(info.Awake());
        ret->AwakeInfo(info.AwakeInfo());
        ret->NickName(info.NickName());
        ret->AddExtendData("Visitor_location", info.Visitor_location_city().isEmpty() ? info.Visitor_location_province() : QString("%1%2").arg(info.Visitor_location_city()).arg(info.Visitor_location_district()));
        ret->AddExtendData("AiChatFlag", QString::number(info.AiChatFlag()));
        ret->AddExtendData("Device", info.Chat_type());
        ret->AddExtendData("CustomerID", info.CustomerID());
        ret->AddExtendData("CustomerName", info.CustomerName());
        ret->AddExtendData("TokeOverID", info.TokeOverID());

        ret->AddExtendData("PreCustomerId", info.PreCustomerId());
        ret->AddExtendData("CreateTime", info.CreateTime());
        if (info.TempChat())
        {
            ret->AddExtendData("TempChat", "1");
        }
        else
        {
            ret->AddExtendData("TempChat", "0");
        }
        ret->AddExtendData("ip", info.Visitor_ip());
        ret->AddExtendData("UserMsgCount", info.UserMsgCount());
        ret->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());

        if (userInfo->OtherInfos())
        {
            ret->AddExtendData("SearchEngineId", userInfo->OtherInfos()->GetValueByKey("searchEngineId"));
            ret->AddExtendData("groupId", userInfo->OtherInfos()->GetValueByKey("groupId"));
            ret->AddExtendData("keyword", userInfo->OtherInfos()->GetValueByKey("keyWord"));
            ret->AddExtendData("xst", userInfo->OtherInfos()->GetValueByKey("xst"));
            ret->AddExtendData("fanConversationId", userInfo->OtherInfos()->GetValueByKey("fanConversationId")); //抖音粉丝群
            DATAADDEXTENDDATA(ret, userInfo->OtherInfos());

        }
        if (userInfo->CardInfo())
        {
            ret->AddExtendData("hasCard", "1");
            ret->AddExtendData("cardName", userInfo->CardInfo()->Name());
            ret->AddExtendData("cardTel", userInfo->CardInfo()->Tel());
            ret->AddExtendData("cardMobile", userInfo->CardInfo()->Mobile());
            ret->AddExtendData("cardPhoneStatus", userInfo->CardInfo()->PhoneStatus());
            ret->AddExtendData("cardNote", userInfo->CardInfo()->Note());
            ret->AddExtendData("cardWX", userInfo->CardInfo()->Msn());
            ret->AddExtendData("cardQQ", userInfo->CardInfo()->Qq());
            if (userInfo->CardInfo()->IsHasContact())
            {
                ret->AddExtendData("IsHasContact", "1");
            }
            else
            {
                ret->AddExtendData("IsHasContact", "0");
            }
            if (userInfo->CardInfo()->IsAiEdit())
            {
                ret->AddExtendData("IsAiEditIsAiEdit", "1");
            }
            else
            {
                ret->AddExtendData("IsAiEdit", "1");
            }
        }
        else
        {
            ret->AddExtendData("hasCard", "0");
            ret->AddExtendData("IsHasContact", "0");
            ret->AddExtendData("IsAiEdit", "0");
        }
    }
    return ret;
}


MChatItemData* UIBaseFactory::CreateMChatItemData(const UIWaitVisitorInfo& info)
{
    MChatItemData* ret = nullptr;
    ret                = new MChatItemData();
    QString strName    = gUICacheService->GetDisplayName(&info);

    ret->InitData(info.UserId(), info.ViewVid(), "", strName, E_VISITOR, E_CHATITEM_BROWSE);
    ret->AddExtendData("JoinQueueTime", info.JoinQueueTime());
    ret->AddExtendData("GroupId", info.GroupId());
    ret->AddExtendData("ip", info.Visitor_ip());
    ret->AddExtendData("ipv6", info.Visitor_ipv6());
    ret->AddExtendData("Device", info.Chat_type());
    if (info.CardInfo())
    {
        ret->AddExtendData("hasCard", "1");
        ret->AddExtendData("cardName", info.CardInfo()->Name());
        ret->AddExtendData("cardTel", info.CardInfo()->Tel());
        ret->AddExtendData("cardMobile", info.CardInfo()->Mobile());
        ret->AddExtendData("cardPhoneStatus", info.CardInfo()->PhoneStatus());
        ret->AddExtendData("cardNote", info.CardInfo()->Note());
        ret->AddExtendData("cardWX", info.CardInfo()->Msn());
        ret->AddExtendData("cardQQ", info.CardInfo()->Qq());
        if (info.CardInfo()->IsHasContact())
        {
            ret->AddExtendData("IsHasContact", "1");
        }
        else
        {
            ret->AddExtendData("IsHasContact", "0");
        }
        if (info.CardInfo()->IsAiEdit())
        {
            ret->AddExtendData("IsAiEdit", "1");
        }
        else
        {
            ret->AddExtendData("IsAiEdit", "0");
        }
    }
    else
    {
        ret->AddExtendData("hasCard", "0");
        ret->AddExtendData("IsHasContact", "0");
        ret->AddExtendData("IsAiEdit", "1");
    }
    return ret;
}

MChatItemData* UIBaseFactory::CreateMChatItemData(const UIContactInfo& info)
{
    MChatItemData* ret = nullptr;

    ret             = new MChatItemData();
    QString strName = info.DisplayInnerName();
    ret->InitData(info.UserId(), info.UserId(), info.UserId(), strName, E_CUSTOMER, E_CHATITEM_CHAT);
    ret->AddExtendData("status", QString::number((int)info.Status()));
    ret->AddExtendData("loginType", QString::number((int)info.LoginType()));
    ret->AddExtendData("AiFlag", QString::number(info.AiFlag()));
    return ret;
}

MChatItemData* UIBaseFactory::CreateMChatItemData(const UIVisitorInfo& info)
{
    MChatItemData* ret = nullptr;

    ret             = new MChatItemData();
    QString strName = gUICacheService->GetDisplayName(&info);

    ret->InitData(info.Vid(), info.StaticId(), "", strName, E_VISITOR, E_CHATITEM_BROWSE);
    ret->SearchingType(info.SearchingType());
    ret->Awake(info.Awake());
    ret->AwakeInfo(info.AwakeInfo());
    ret->NickName(info.NickName());

    ret->AddExtendData("Device", info.Device());
    ret->AddExtendData("SearchEngineId", info.SearchEngineId());
    ret->AddExtendData("visitor_keyWord", info.KeyWord());
    ret->AddExtendData("visitor_location", info.LocalOption());
    ret->AddExtendData("visitor_Name", gUICacheService->GetDisplayName(&info));
    ret->AddExtendData("ip", info.Ip());
    ret->AddExtendData("visitor_hasCard", info.HasCard());
    ret->AddExtendData("visitorCount", info.VisitorCount());
    ret->AddExtendData("preCustomerId", info.PreCustomerId());

    ret->AddExtendData(DOUYI_HEADIMGURL, info.HeadImgUrl());
    ret->AddExtendData(DOUYI_GENDER, info.Gender());

    if (info.CardInfo())
    {
        ret->AddExtendData("hasCard", "1");
        ret->AddExtendData("cardName", info.CardInfo()->Name());
        ret->AddExtendData("cardTel", info.CardInfo()->Tel());
        ret->AddExtendData("cardMobile", info.CardInfo()->Mobile());
        ret->AddExtendData("cardPhoneStatus", info.CardInfo()->PhoneStatus());
        ret->AddExtendData("cardNote", info.CardInfo()->Note());
        ret->AddExtendData("cardWX", info.CardInfo()->Msn());
        ret->AddExtendData("cardQQ", info.CardInfo()->Qq());
        if (info.CardInfo()->IsHasContact())
        {
            ret->AddExtendData("IsHasContact", "1");
        }
        else
        {
            ret->AddExtendData("IsHasContact", "0");
        }
        if (info.CardInfo()->IsAiEdit())
        {
            ret->AddExtendData("IsAiEdit", "1");
        }
        else
        {
            ret->AddExtendData("IsAiEdit", "0");
        }
    }
    else
    {
        ret->AddExtendData("hasCard", "0");
        ret->AddExtendData("IsHasContact", "0");
        ret->AddExtendData("IsAiEdit", "1");
    }
    return ret;
}

MChatItemData* UIBaseFactory::CreateMChatItemData(const UIChatInfo& info, ECHATITEMSTATE state)
{
    MChatItemData*  ret      = nullptr;
    UIChatUserInfo* userInfo = info.GetVisitorInfo();
    if (userInfo)
    {
        ret = new MChatItemData();
        if (state == E_CHATITEM_MONITOR_CHAT || state == E_CHATITEM_MONITOR_CHAT_CLOSE)
        {
            QString customerId   = info.CustomerID();
            QString customerName = info.CustomerName();
            QString strName      = gUICacheService->GetDisplayName(&info);

            ret->InitData(userInfo->UserId(), userInfo->StaticId(), info.ChatId(), strName, E_MONITOR, state);
            ret->NewChat(info.NewChat());
            ret->InviteMode(info.InviteMode());
            ret->SearchingType(info.SearchingType());
            ret->Awake(info.Awake());
            ret->AwakeInfo(info.AwakeInfo());
            ret->NickName(info.NickName());
            ret->EffectiveAllocate(info.EffectiveAllocate());
            ret->MsgType(info.MsgType());

            ret->AddExtendData("Visitor_location", info.Visitor_location_city().isEmpty() ? info.Visitor_location_province() : QString("%1%2").arg(info.Visitor_location_city()).arg(info.Visitor_location_district()));
            ret->AddExtendData("AiChatFlag", QString::number(info.AiChatFlag()));
            ret->AddExtendData("ip", info.Visitor_ip());
            ret->AddExtendData("ipv6", info.Visitor_ipv6());
            ret->AddExtendData("CustomerID", info.CustomerID());
            ret->AddExtendData("TokeOverID", info.TokeOverID());
            ret->AddExtendData("CustomerName", info.CustomerName());
            ret->AddExtendData("PreCustomerId", info.PreCustomerId());
            ret->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
            ret->AddExtendData("UserMsgCount", info.UserMsgCount());
            ret->AddExtendData("Device", info.Chat_type());
            ret->AddExtendData("CreateTime", info.CreateTime());

            if (userInfo->OtherInfos())
            {
                ret->AddExtendData("SearchEngineId", userInfo->OtherInfos()->GetValueByKey("searchEngineId"));
                ret->AddExtendData("groupId", userInfo->OtherInfos()->GetValueByKey("groupId"));
                ret->AddExtendData("keyword", userInfo->OtherInfos()->GetValueByKey("keyWord"));
                ret->AddExtendData("xst", userInfo->OtherInfos()->GetValueByKey("xst"));
                ret->AddExtendData("fanConversationId", userInfo->OtherInfos()->GetValueByKey("fanConversationId")); //抖音粉丝群
                DATAADDEXTENDDATA(ret, userInfo->OtherInfos());

            }
            if (userInfo->CardInfo())
            {
                ret->AddExtendData("hasCard", "1");
                ret->AddExtendData("cardName", userInfo->CardInfo()->Name());
                ret->AddExtendData("cardTel", userInfo->CardInfo()->Tel());
                ret->AddExtendData("cardMobile", userInfo->CardInfo()->Mobile());
                ret->AddExtendData("cardPhoneStatus", userInfo->CardInfo()->PhoneStatus());
                ret->AddExtendData("cardNote", userInfo->CardInfo()->Note());
                ret->AddExtendData("cardWX", userInfo->CardInfo()->Msn());
                ret->AddExtendData("cardQQ", userInfo->CardInfo()->Qq());

                if (userInfo->CardInfo()->IsHasContact())
                {
                    ret->AddExtendData("IsHasContact", "1");
                }
                else
                {
                    ret->AddExtendData("IsHasContact", "0");
                }
                if (userInfo->CardInfo()->IsAiEdit())
                {
                    ret->AddExtendData("IsAiEdit", "1");
                }
                else
                {
                    ret->AddExtendData("IsAiEdit", "0");
                }
            }
            else
            {
                ret->AddExtendData("hasCard", "0");
                ret->AddExtendData("IsHasContact", "0");
                ret->AddExtendData("IsAiEdit", "1");
            }
        }
        else
        {
            QString strName = gUICacheService->GetDisplayName(&info);
            if (strName.isEmpty())
            {
                UIVisitorInfo* visitorData = (UIVisitorInfo*)gUIVisitorInfoManager->GetVisitorInfoById(userInfo->UserId(), ConstStringHelper::m_cUIVISITORINFO, this);
                if (visitorData)
                {
                    strName = gUICacheService->GetDisplayName(visitorData);
                    visitorData->deleteLater();
                }
            }

            ret->InitData(userInfo->UserId(), userInfo->StaticId(), info.ChatId(), strName, E_VISITOR, state);
            ret->SearchingType(info.SearchingType());
            ret->Awake(info.Awake());
            ret->AwakeInfo(info.AwakeInfo());
            ret->NickName(info.NickName());
            ret->EffectiveAllocate(info.EffectiveAllocate());
            ret->MsgType(info.MsgType());

            ret->AddExtendData("ip", info.Visitor_ip());
            ret->AddExtendData("ipv6", info.Visitor_ipv6());
            ret->AddExtendData("Visitor_location", info.Visitor_location_city().isEmpty() ? info.Visitor_location_province() : QString("%1%2").arg(info.Visitor_location_city()).arg(info.Visitor_location_district()));
            ret->AddExtendData("AiChatFlag", QString::number(info.AiChatFlag()));
            ret->AddExtendData("CustomerID", info.CustomerID());
            ret->AddExtendData("TokeOverID", info.TokeOverID());
            ret->AddExtendData("PreCustomerId", info.PreCustomerId());
            ret->AddExtendData("isAckLeave", info.IsAckLeave() ? "1" : "0");
            ret->AddExtendData("takeoverFlag", QString("%1").arg(info.TakeoverFlag()));
            ret->AddExtendData("CustomerName", info.CustomerName());
            ret->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
            ret->AddExtendData("UserMsgCount", info.UserMsgCount());
            ret->AddExtendData("Device", info.Chat_type());
            ret->AddExtendData("CreateTime", info.CreateTime());

            if (userInfo->OtherInfos())
            {
                ret->AddExtendData("SearchEngineId", userInfo->OtherInfos()->GetValueByKey("searchEngineId"));
                ret->AddExtendData("groupId", userInfo->OtherInfos()->GetValueByKey("groupId"));
                ret->AddExtendData("keyword", userInfo->OtherInfos()->GetValueByKey("keyWord"));
                ret->AddExtendData("xst", userInfo->OtherInfos()->GetValueByKey("xst"));
                ret->AddExtendData("fanConversationId", userInfo->OtherInfos()->GetValueByKey("fanConversationId"));
                DATAADDEXTENDDATA(ret, userInfo->OtherInfos());

            }

            if (userInfo->CardInfo())
            {
                ret->AddExtendData("hasCard", "1");
                ret->AddExtendData("cardName", userInfo->CardInfo()->Name());
                ret->AddExtendData("cardTel", userInfo->CardInfo()->Tel());
                ret->AddExtendData("cardMobile", userInfo->CardInfo()->Mobile());
                ret->AddExtendData("cardPhoneStatus", userInfo->CardInfo()->PhoneStatus());
                ret->AddExtendData("cardNote", userInfo->CardInfo()->Note());
                ret->AddExtendData("cardWX", userInfo->CardInfo()->Msn());
                ret->AddExtendData("cardQQ", userInfo->CardInfo()->Qq());
                if (userInfo->CardInfo()->IsHasContact())
                {
                    ret->AddExtendData("IsHasContact", "1");
                }
                else
                {
                    ret->AddExtendData("IsHasContact", "0");
                }
                if (userInfo->CardInfo()->IsAiEdit())
                {
                    ret->AddExtendData("IsAiEdit", "1");
                }
                else
                {
                    ret->AddExtendData("IsAiEdit", "0");
                }
            }
            else
            {
                ret->AddExtendData("hasCard", "0");
                ret->AddExtendData("IsHasContact", "0");
                ret->AddExtendData("IsAiEdit", "1");
            }
        }
        if (ret)
        {
            ret->NewChat(info.NewChat());
            ret->InviteMode(info.InviteMode());
            ret->EffectiveAllocate(info.EffectiveAllocate());
            ret->MsgType(info.MsgType());
            if (info.TempChat())
            {
                ret->AddExtendData("TempChat", "1");
            }
            else
            {
                ret->AddExtendData("TempChat", "0");
            }
        }
    }
    return ret;
}

MChatItemData* UIBaseFactory::CreateMChatItemData(const UIChatInfo& info)
{
    MChatItemData* ret = nullptr;

    UIChatUserInfo* userInfo = info.GetVisitorInfo();
    if (userInfo)
    {
        if (info.CustomerID() == gSessionService->UserInfo()->UserId())
        {
            ret             = new MChatItemData();
            QString strName = gUICacheService->GetDisplayName(&info);
            if (info.TempChat())
            {
                ret->InitData(userInfo->UserId(), userInfo->StaticId(), info.ChatId(), strName, E_VISITOR, E_CHATITEM_INVITE);
            }
            else
            {
                ret->InitData(userInfo->UserId(), userInfo->StaticId(), info.ChatId(), strName, E_VISITOR, E_CHATITEM_CHAT);
            }
            ret->SearchingType(info.SearchingType());
            ret->Awake(info.Awake());
            ret->AwakeInfo(info.AwakeInfo());
            ret->NickName(info.NickName());
            ret->MsgType(info.MsgType());

            ret->AddExtendData("Visitor_location", info.Visitor_location_city().isEmpty() ? info.Visitor_location_province() : QString("%1%2").arg(info.Visitor_location_city()).arg(info.Visitor_location_district()));
            ret->AddExtendData("AiChatFlag", QString::number(info.AiChatFlag()));
            ret->AddExtendData("ip", info.Visitor_ip());
            ret->AddExtendData("ipv6", info.Visitor_ipv6());
            ret->AddExtendData("CustomerID", info.CustomerID());
            ret->AddExtendData("TokeOverID", info.TokeOverID());
            ret->AddExtendData("CustomerName", info.CustomerName());
            ret->AddExtendData("PreCustomerId", info.PreCustomerId());
            ret->AddExtendData("isAckLeave", info.IsAckLeave() ? "1" : "0");
            ret->AddExtendData("takeoverFlag", QString("%1").arg(info.TakeoverFlag()));
            ret->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
            ret->AddExtendData("UserMsgCount", info.UserMsgCount());
            ret->AddExtendData("Device", info.Chat_type());
            ret->AddExtendData("CreateTime", info.CreateTime());
            ret->AddExtendData("mychat", "1");

            if (userInfo->OtherInfos())
            {
                ret->AddExtendData("SearchEngineId", userInfo->OtherInfos()->GetValueByKey("searchEngineId"));
                ret->AddExtendData("groupId", userInfo->OtherInfos()->GetValueByKey("groupId"));
                ret->AddExtendData("keyword", userInfo->OtherInfos()->GetValueByKey("keyWord"));
                ret->AddExtendData("xst", userInfo->OtherInfos()->GetValueByKey("xst"));
                ret->AddExtendData("fanConversationId", userInfo->OtherInfos()->GetValueByKey("fanConversationId"));
                DATAADDEXTENDDATA(ret, userInfo->OtherInfos());

            }
            if (userInfo->CardInfo())
            {
                ret->AddExtendData("hasCard", "1");
                ret->AddExtendData("cardName", userInfo->CardInfo()->Name());
                ret->AddExtendData("cardTel", userInfo->CardInfo()->Tel());
                ret->AddExtendData("cardMobile", userInfo->CardInfo()->Mobile());
                ret->AddExtendData("cardPhoneStatus", userInfo->CardInfo()->PhoneStatus());
                ret->AddExtendData("cardNote", userInfo->CardInfo()->Note());
                ret->AddExtendData("cardWX", userInfo->CardInfo()->Msn());
                ret->AddExtendData("cardQQ", userInfo->CardInfo()->Qq());
                if (userInfo->CardInfo()->IsHasContact())
                {
                    ret->AddExtendData("IsHasContact", "1");
                }
                else
                {
                    ret->AddExtendData("IsHasContact", "0");
                }
                if (userInfo->CardInfo()->IsAiEdit())
                {
                    ret->AddExtendData("IsAiEdit", "1");
                }
                else
                {
                    ret->AddExtendData("IsAiEdit", "0");
                }
            }
            else
            {
                ret->AddExtendData("hasCard", "0");
                ret->AddExtendData("IsHasContact", "0");
                ret->AddExtendData("IsAiEdit", "1");
            }
        }
        else
        {
            if (info.IsWatch(gSessionService->UserInfo()->UserId()))
            {
                ret                  = new MChatItemData();
                QString customerId   = info.CustomerID();
                QString customerName = info.CustomerName();
                QString strName      = gUICacheService->GetDisplayName(&info);
                /*	if (!customerName.isEmpty())
                        {
                                strName = QString("%1%2").arg(customerName).arg(info.ChatId().right(4));
                        }
                        else
                        {
                                strName = QString("%1%2").arg(customerId).arg(info.ChatId().right(4));
                        }*/
                ret->InitData(userInfo->UserId(), userInfo->StaticId(), info.ChatId(), strName, E_MONITOR, E_CHATITEM_MONITOR_CHAT);
                ret->SearchingType(info.SearchingType());
                ret->Awake(info.Awake());
                ret->AwakeInfo(info.AwakeInfo());
                ret->NickName(info.NickName());
                ret->MsgType(info.MsgType());
                ret->AddExtendData("Visitor_location", info.Visitor_location_city().isEmpty() ? info.Visitor_location_province() : QString("%1%2").arg(info.Visitor_location_city()).arg(info.Visitor_location_district()));
                ret->AddExtendData("AiChatFlag", QString::number(info.AiChatFlag()));
                ret->AddExtendData("ip", info.Visitor_ip());
                ret->AddExtendData("ipv6", info.Visitor_ipv6());
                ret->AddExtendData("CustomerID", info.CustomerID());
                ret->AddExtendData("TokeOverID", info.TokeOverID());
                ret->AddExtendData("CustomerName", info.CustomerName());
                ret->AddExtendData("PreCustomerId", info.PreCustomerId());
                ret->AddExtendData("isAckLeave", info.IsAckLeave() ? "1" : "0");
                ret->AddExtendData("takeoverFlag", QString("%1").arg(info.TakeoverFlag()));
                ret->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
                ret->AddExtendData("UserMsgCount", info.UserMsgCount());
                ret->AddExtendData("Device", info.Chat_type());
                ret->AddExtendData("CreateTime", info.CreateTime());
                ret->AddExtendData("mychat", "0");

                if (userInfo->OtherInfos())
                {
                    ret->AddExtendData("SearchEngineId", userInfo->OtherInfos()->GetValueByKey("searchEngineId"));
                    ret->AddExtendData("groupId", userInfo->OtherInfos()->GetValueByKey("groupId"));
                    ret->AddExtendData("keyword", userInfo->OtherInfos()->GetValueByKey("keyWord"));
                    ret->AddExtendData("xst", userInfo->OtherInfos()->GetValueByKey("xst"));
                    ret->AddExtendData("fanConversationId", userInfo->OtherInfos()->GetValueByKey("fanConversationId"));
                    DATAADDEXTENDDATA(ret, userInfo->OtherInfos());

                }
                if (userInfo->CardInfo())
                {
                    ret->AddExtendData("hasCard", "1");
                    ret->AddExtendData("cardName", userInfo->CardInfo()->Name());
                    ret->AddExtendData("cardTel", userInfo->CardInfo()->Tel());
                    ret->AddExtendData("cardMobile", userInfo->CardInfo()->Mobile());
                    ret->AddExtendData("cardPhoneStatus", userInfo->CardInfo()->PhoneStatus());
                    ret->AddExtendData("cardNote", userInfo->CardInfo()->Note());
                    ret->AddExtendData("cardWX", userInfo->CardInfo()->Msn());
                    ret->AddExtendData("cardQQ", userInfo->CardInfo()->Qq());
                    if (userInfo->CardInfo()->IsHasContact())
                    {
                        ret->AddExtendData("IsHasContact", "1");
                    }
                    else
                    {
                        ret->AddExtendData("IsHasContact", "0");
                    }

                    if (userInfo->CardInfo()->IsAiEdit())
                    {
                        ret->AddExtendData("IsAiEdit", "1");
                    }
                    else
                    {
                        ret->AddExtendData("IsAiEdit", "0");
                    }
                }
                else
                {
                    ret->AddExtendData("hasCard", "0");
                    ret->AddExtendData("IsHasContact", "0");
                    ret->AddExtendData("IsAiEdit", "1");
                }
            }
            else
            {
                // do nothing。
            }
        }
        if (ret)
        {
            ret->InviteMode(info.InviteMode());
            ret->NewChat(info.NewChat());
            ret->MsgType(info.MsgType());
            if (info.TempChat())
            {
                ret->AddExtendData("TempChat", "1");
            }
            else
            {
                ret->AddExtendData("TempChat", "0");
            }
        }
    }
    return ret;
}

void UIBaseFactory::AddConstructionExt(ConstructionDataInterface * obj )
{
    ConstructionData::AddConstructionExt(obj);
}
