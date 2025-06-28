#include "UIContactTreeModel.h"

#include "utils/ConstStringHelper.h"
#include "utils/EL_Util_Time.h"
#include "model/uiplatcommondatamodel.h"
#include "service/platcommonmodelservice.h"
#include "service/PlatCommonSignalService.h"
#include "serviceinterface/dbuiplatservice.h"


#include "data/UIContactGroupInfo.h"
#include "session/el_sessionservice.h"
#include "utils/utilshelper.h"

UIContactTreeModel::UIContactTreeModel()
{
    m_currentUserId = "";
}

UIContactTreeModel::~UIContactTreeModel()
{
}

void UIContactTreeModel::SetCurrentUserId(QString curUserId)
{
    m_currentUserId = curUserId;
}

void UIContactTreeModel::Init()
{
    //////////////////////////////////////////////////////////////////////////
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CreateTempChat(UIChatInfo*)), this, SLOT(R_CreateTempChat(UIChatInfo*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_ShowColleagueList()), this, SLOT(R_ShowColleagueList()));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_GetVisitorInfoResult(QString, UICardInfo*, int)), this, SLOT(R_GetVisitorInfoResult(QString, UICardInfo*, int)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_GetWaitQueueEnd()), this, SLOT(R_GetWaitQueueEnd()));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CustomerPresenceChanged(QString, EPRESENCESTATUS, EPRESENCESTATUS, CUSTOMER_LOGIN_TYPE)), this, SLOT(R_CustomerPresenceChanged(QString, EPRESENCESTATUS, EPRESENCESTATUS, CUSTOMER_LOGIN_TYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_AddWaitQueueInfo(UIWaitVisitorInfo*)), this, SLOT(R_AddWaitQueueInfo(UIWaitVisitorInfo*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_RemoveWaitQueueInfoByVid(QString)), this, SLOT(R_RemoveWaitQueueInfoByVid(QString)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_SetVisitorStatus(QString, EVISITORSTATUS, EVISITORSTATUS)), this, SLOT(R_SetVisitorStatus(QString, EVISITORSTATUS, EVISITORSTATUS)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_RemoveVMVisitorList(const QStringList&)), this, SLOT(R_RemoveVMVisitorList(const QStringList&)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CreateTransferChat(UIChatInfo*, QString)), this, SLOT(R_CreateTransferChat(UIChatInfo*, QString)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_RemoveTempVMVisitorList(const QStringList&)), this, SLOT(R_RemoveTempVMVisitorList(const QStringList&)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_AddVisitorInfo(UIVisitorInfo*)), this, SLOT(R_AddVisitorInfo(UIVisitorInfo*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_ReceiveChatCloseMsg(QString, QString, QString)), this, SLOT(R_ReceiveChatCloseMsg(QString, QString, QString)));

    //////////////////////////////////////////////////////////////////////////

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnJYMainTreeMenuClick(QString, MChatItemData*, EUINODEMODELTYPE)), this, SLOT(R_OnJYMainTreeMenuClick(QString, MChatItemData*, EUINODEMODELTYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnDoubleClickChatItemData(MChatItemData*, EUINODEMODELTYPE)), this, SLOT(R_OnDoubleClickChatItemData(MChatItemData*, EUINODEMODELTYPE)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnContactDoubleClick(MContactItemData*)), this, SLOT(R_OnContactDoubleClick(MContactItemData*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnCommonTreeMenuClick(QString, MContactItemData*)), this, SLOT(R_OnCommonTreeMenuClick(QString, MContactItemData*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnChatItemCloseClicked(QString, QString)), this, SLOT(R_OnChatItemCloseClicked(QString, QString)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_AddChat(UIChatInfo)), this, SLOT(R_AddChat(UIChatInfo)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_UpdateChat(UIChatInfo)), this, SLOT(R_UpdateChat(UIChatInfo)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_DelChat(QString, QString)), this, SLOT(R_DelChat(QString, QString)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_AddVisitor(UIVisitorInfo)), this, SLOT(R_AddVisitor(UIVisitorInfo)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_UpdateVisitor(UIVisitorInfo)), this, SLOT(R_UpdateVisitor(UIVisitorInfo)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_DelVisitor(QString)), this, SLOT(R_DelVisitor(QString)));
}

void UIContactTreeModel::AddColleagueList()
{

    QMap<QString, MContactItemData*> grouplist;
    gUIColleagueManager->GetColleagueGroupList("E_UI_MODEL_COLLEAGUE_GROUP-GROUPMAP", this, &grouplist);

    for (auto itor = grouplist.begin(); itor != grouplist.end(); itor++)
    {
        MContactItemData* group = itor.value();
        if (group)
        {
            m_modelContactItemDataList.append(group);
            m_modelContactItemDataIndexList.insert(group->GetUUid(), group);
            NotifyObjectChange("ADD_E_UI_MODEL_COLLEAGUE_GROUP", group);

            QList<MContactItemData*> memberList;
            gUIColleagueManager->GetColleagueMemberListByGroupID(group->DepartmentId(), "E_UI_MODEL_COLLEAGUE_CUSTOMER-LIST", this, &memberList);

            int onlineNumber = 0;
            int allcount     = 0;
            Q_FOREACH (MContactItemData* member, memberList)
            {
                if (member)
                {
                    m_modelContactItemDataList.append(member);
                    m_modelContactItemDataIndexList.insert(member->GetUUid(), member);
                    NotifyObjectChange("ADD_E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER", member);
                    allcount++;

                    EPRESENCESTATUS status = (EPRESENCESTATUS)member->GetExtendData("Status").toInt();
                    if (status == E_ONLINE || status == E_BUSY || status == E_LEAVE)
                    {
                        onlineNumber++;
                    }
                }
            }
            group->AddExtendData("allcount", QString::number(allcount));
            group->AddExtendData("online", QString::number(onlineNumber));
            group->AddExtendData("UICONTACTTREEMODEL_CMD", "onlinecount");
            NotifyObjectChange("UPDATE_E_UI_MODEL_COLLEAGUE_GROUP", group);
        }
    }
}

void UIContactTreeModel::AddMonitorVisitorList()
{
    return;
    QStringList tempIdList = gUIVisitorInfoManager->TempIdList();

    QMap<QString, MContactItemData*> onlineMap;
    gUIVisitorInfoManager->GetCurrentVisitorInfo("E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR_VISITORMAP", this, &onlineMap);

    for (auto itor = onlineMap.begin(); itor != onlineMap.end(); itor++)
    {
        QString           id      = itor.key();
        MContactItemData* itemObj = itor.value();
        if (itemObj == NULL)
        {
            continue;
        }
        if (tempIdList.indexOf(itemObj->Vid()) != -1)
        {
            continue;
        }

        if (IsFindContactItemNodeById(id, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR))
        {
            // do nothing;
            MContactItemData* data = GetContactItemNodeById(id, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
        }
        else
        {
            m_modelContactItemDataList.append(itemObj);
            m_modelContactItemDataIndexList.insert(id, itemObj);
            NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", itemObj);
        }

        if (IsFindContactItemNodeById(id, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR))
        {
            MContactItemData* data = GetContactItemNodeById(id, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);
            if (data)
            {

                NotifyObjectChange("REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", data);
                RemoveModelCacheById(id, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);
            }
        }
        else
        {
            // do nothing;
        }
    }
}
void UIContactTreeModel::AddQueueVisitor(const UIWaitVisitorInfo& info)
{
    if (IsFindContactItemNodeById(info.UserId(), E_UI_MODEL_QUEUE_VISITOR))
    {
        // do nothing.
    }
    else
    {
        MContactItemData* data = BuilderContactItemData(info, E_UI_MODEL_QUEUE_VISITOR);
        if (data)
        {
            NotifyObjectChange("ADD_E_UI_MODEL_QUEUE_VISITOR", data);
        }
    }
}

void UIContactTreeModel::RemoveQueueVisitor(const UIWaitVisitorInfo& info)
{
    RemoveQueueVisitor(info.UserId());
}

void UIContactTreeModel::RemoveQueueVisitor(QString vid)
{
    NotifyRemoveObject("REMOVE_E_UI_MODEL_QUEUE_VISITOR", vid);
    RemoveModelCacheById(vid, E_UI_MODEL_QUEUE_VISITOR);
}

void UIContactTreeModel::AddChatInfo(const UIChatInfo& info)
{
    bool isAddChat = false;
    if (IsFindContactItemNodeById(info.CustomerID(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE))
    {
        //更新对话总数

        UIChatUserInfo* userInfo = info.GetVisitorInfo();
        if (userInfo)
        {
            QString isShow = gUISystemConfigManager->GetSystemConfigByKey(ConstStringHelper::m_cNoShowVisitor, "0");
            if ((isShow != "0") || (info.VisitorMsgCount() != "0"))
            {

                if (IsFindContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
                {

                    MContactItemData* chatData = GetContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
                    if (chatData)
                    {
                        chatData->AddExtendData("CustomerID", info.CustomerID());
                        chatData->AddExtendData("TokeOverID", info.TokeOverID());
                        chatData->AddExtendData("UserMsgCount", info.UserMsgCount());
                        chatData->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
                        chatData->AddExtendData("UICONTACTTREEMODEL_CMD", "msgcount");

                        NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", chatData);
                    }
                }
                else
                {
                    MContactItemData* data = BuilderContactItemData(info, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
                    m_modelContactItemDataList.append(data);
                    m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
                    m_modelCustomerContactItemDataIndexList.insert(info.CustomerID(), data);
                    if (data)
                    {
                        data->AddExtendData("TokeOverID", info.TokeOverID());
                        data->AddExtendData("CustomerID", info.CustomerID());
                        data->AddExtendData("UserMsgCount", info.UserMsgCount());
                        data->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());

                        NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", data);
                        isAddChat = true;
                    }
                }
            }
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        MContactItemData* data = CreateContactItemData(info.CustomerID(), info.CustomerID(), info.CustomerID(), info.ChatId(), "", "", E_UI_MODEL_MONITOR_CHAT_COLLEAGUE, info.CustomerName());
        if (data)
        {
            data->AddExtendData("Status", QString::number((int)gUIColleagueManager->GetPresenceStatusByID(info.CustomerID())));
            NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", data);
            /*MContactItemData* data =(MContactItemData*)gUIColleagueManager->GetColleagueMemberByID(info.CustomerID(), "E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", this);
            if (data)
            {
                    m_modelContactItemDataList.append(data);
                    m_modelContactItemDataIndexList.insert(data->GetUUid(), data);

                    NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", data);*/
            UIChatUserInfo* userInfo = info.GetVisitorInfo();

            if ((userInfo == NULL) || (IsFindContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)))
            {
                // do nothing.
            }
            else
            {
                QString isShow = UIPlatCommonDataModel::Instance()->GetSystemConfigByKey(ConstStringHelper::m_cNoShowVisitor, "0");
                if ((isShow != "0") || (info.VisitorMsgCount() != "0"))
                {

                    MContactItemData* dataObj = BuilderContactItemData(info, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
                    m_modelContactItemDataList.append(dataObj);
                    m_modelContactItemDataIndexList.insert(dataObj->GetUUid(), dataObj);

                    m_modelCustomerContactItemDataIndexList.insert(info.CustomerID(), dataObj);

                    if (dataObj)
                    {
                        dataObj->AddExtendData("TokeOverID", info.TokeOverID());
                        dataObj->AddExtendData("CustomerID", info.CustomerID());
                        dataObj->AddExtendData("UserMsgCount", info.UserMsgCount());
                        dataObj->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
                        NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", dataObj);
                        isAddChat = true;
                    }
                }
            }
        }
    }

    //更新对话总数
    if (isAddChat)
    {
        UpdateMonitorChatColleague(info.CustomerID());
    }
}

void UIContactTreeModel::RemoveChatInfo(const UIChatInfo& info)
{
    if (IsFindContactItemNodeById(info.CustomerID(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE))
    {
        UIChatUserInfo* userInfo = info.GetVisitorInfo();
        if (userInfo)
        {
            if (IsFindContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
            {
                MContactItemData* data = GetContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
                if (data)
                {
                    RemoveChatColleagueVisitor(data->GetUUid());
                    UpdateMonitorChatColleague(data->GetExtendData("CustomerID"), false);
                }
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        // do nothing
    }
}

void UIContactTreeModel::RemoveChatInfo(const QString& vid, const QString& chatId)
{
    if (IsFindContactItemNodeById(vid, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
    {
        MContactItemData* data = GetContactItemNodeById(vid, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
        if (data)
        {
            if (data->Chatid() != chatId)
            {
                //不应该这样。
                Q_ASSERT(false);
            }
            RemoveChatColleagueVisitor(data->GetUUid());
            UpdateMonitorChatColleague(data->GetExtendData("CustomerID"), false);
        }
    }
    else
    {
        // do nothing

        UIChatInfo* info = UIPlatCommonDataModel::Instance()->GetChatByChatId(chatId);
        if (info)
        {
            QString visitorId = "";
            if (info->GetVisitorInfo())
            {
                visitorId = info->GetVisitorInfo()->UserId();
            }
            if (visitorId.isEmpty())
            {
                return;
            }
            if (IsFindContactItemNodeById(visitorId, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
            {
                MContactItemData* data = GetContactItemNodeById(visitorId, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
                if (data)
                {
                    if (data->Chatid() != chatId)
                    {
                        //不应该这样。
                        Q_ASSERT(false);
                    }
                    RemoveChatColleagueVisitor(data->GetUUid());
                    UpdateMonitorChatColleague(data->GetExtendData("CustomerID"), false);
                }
            }
        }
    }
}

void UIContactTreeModel::AddMonitorVisitorInfo(UIVisitorInfo& info)
{
    NotifyOnlineMonitorVisitor("ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", info);
    NotifyOfflineMonitorVisitor("REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", info);
}

void UIContactTreeModel::RemoveMonitorVisitorInfo(UIVisitorInfo& info)
{
    NotifyOnlineMonitorVisitor("REMOVE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", info);
    NotifyOfflineMonitorVisitor("ADD_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", info);
}

void UIContactTreeModel::DeleteMonitorVisitorInfo(UIVisitorInfo& info)
{
    NotifyOnlineMonitorVisitor("DELETE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", info);
    NotifyOfflineMonitorVisitor("DELETE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", info);
}

MContactItemData* UIContactTreeModel::CreateContactItemData(QString strUUid, QString vid, QString userid, QString chatid, QString groupid, QString departmentId, EUINODEMODELTYPE etype, QString strName)
{
    MContactItemData* dataItem = new MContactItemData();
    dataItem->InitData(strUUid, vid, userid, chatid, strName, groupid, departmentId, etype);

    m_modelContactItemDataList.append(dataItem);
    m_modelContactItemDataIndexList.insert(dataItem->GetUUid(), dataItem);
    return dataItem;
}
bool UIContactTreeModel::IsFindContactItemNodeById(const QString& id, EUINODEMODELTYPE eType)
{
    bool                     isFind = false;
    QList<MContactItemData*> list   = m_modelContactItemDataIndexList.values(id);
    Q_FOREACH (MContactItemData* obj, list)
    {
        if (obj && obj->NoteType() == eType)
        {
            isFind = true;
            break;
        }
    }

    return isFind;
}

MContactItemData* UIContactTreeModel::GetContactItemNodeById(const QString& id, EUINODEMODELTYPE eType)
{
    MContactItemData*        ret  = nullptr;
    QList<MContactItemData*> list = m_modelContactItemDataIndexList.values(id);
    Q_FOREACH (MContactItemData* obj, list)
    {
        if (obj && obj->NoteType() == eType)
        {
            ret = obj;
            break;
        }
    }
    return ret;
}

void UIContactTreeModel::NotifyOnlineMonitorVisitor(QString actionType, UIVisitorInfo& info)
{
    if (StringCompare(actionType, "ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
    {
        if (IsFindContactItemNodeById(info.Vid(), E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR))
        {
            // do nothing;
            MContactItemData* data = GetContactItemNodeById(info.Vid(), E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
        }
        else
        {
            MContactItemData* data = BuilderContactItemData(info, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);

            m_modelContactItemDataList.append(data);
            m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
            if (data)
            {
                NotifyObjectChange(actionType, data);
            }
        }
    }
    else if (StringCompare(actionType, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
    {
        NotifyRemoveOnlineMonitorVisitorById(info.Vid(), actionType);
    }
    else if (StringCompare(actionType, "DELETE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
    {
        NotifyDeleteOnlineMonitorVisitorById(info.Vid(), actionType);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UIContactTreeModel::NotifyOnlineMonitorVisitor(QString actionType, MContactItemData& info)
{
    if (StringCompare(actionType, "ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
    {
        NotifyAddOnlineMonitorVisitor(info, actionType);
    }
    else if (StringCompare(actionType, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
    {
        NotifyRemoveOfflineMonitorVisitorById(info.GetUUid(), actionType);
    }
    else if (StringCompare(actionType, "DELETE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
    {
        NotifyDeleteOnlineMonitorVisitorById(info.GetUUid(), actionType);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UIContactTreeModel::NotifyOfflineMonitorVisitor(QString actionType, UIVisitorInfo& info)
{
    if (StringCompare(actionType, "ADD_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
    {
        if (IsFindContactItemNodeById(info.Vid(), E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR))
        {
            // do nothing;
            MContactItemData* data = GetContactItemNodeById(info.Vid(), E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);
        }
        else
        {
            MContactItemData* data = BuilderContactItemData(info, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);
            m_modelContactItemDataList.append(data);
            m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
            if (data)
            {
                NotifyObjectChange(actionType, data);
            }
        }
    }
    else if (StringCompare(actionType, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
    {
        NotifyRemoveOfflineMonitorVisitorById(info.Vid(), actionType);
    }
    else if (StringCompare(actionType, "DELETE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
    {
        NotifyDeleteOfflineMonitorVisitorById(info.Vid(), actionType);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UIContactTreeModel::NotifyOfflineMonitorVisitor(QString actionType, MContactItemData& info)
{
    if (StringCompare(actionType, "ADD_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
    {
        NotifyAddOfflineMonitorVisitor(info, actionType);
    }
    else if (StringCompare(actionType, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
    {
        NotifyRemoveOfflineMonitorVisitorById(info.GetUUid(), actionType);
    }
    else if (StringCompare(actionType, "DELETE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
    {
        NotifyDeleteOfflineMonitorVisitorById(info.GetUUid(), actionType);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UIContactTreeModel::RemoveModelCacheById(QString id, EUINODEMODELTYPE etype)
{
    if (IsFindContactItemNodeById(id, etype))
    {
        MContactItemData* obj = GetContactItemNodeById(id, etype);
        if (obj)
        {
            m_modelContactItemDataIndexList.remove(id, obj);
            m_modelContactItemDataList.removeAll(obj);
            m_modelCustomerContactItemDataIndexList.remove(obj->GetExtendData("CustomerID"), obj);
        }
    }
}

void UIContactTreeModel::AddQueueRootNote()
{
    AddRootNode("ADD_E_UI_MODEL_QUEUE", E_UI_MODEL_QUEUE);
}

void UIContactTreeModel::AddMonitorChatRootNote()
{
    AddRootNode("ADD_E_UI_MODEL_MONITOR_CHAT", E_UI_MODEL_MONITOR_CHAT);
}

void UIContactTreeModel::AddColleagueRootNote()
{
    AddRootNode("ADD_E_UI_MODEL_COLLEAGUE", E_UI_MODEL_COLLEAGUE);
}

void UIContactTreeModel::AddMonitorVisitorRootNote()
{
    AddRootNode("ADD_E_UI_MODEL_MONITOR_VISITOR", E_UI_MODEL_MONITOR_VISITOR);

    AddRootNode("ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE", E_UI_MODEL_MONITOR_VISITOR_ONLINE);

    AddRootNode("ADD_E_UI_MODEL_MONITOR_VISITOR_OFFLINE", E_UI_MODEL_MONITOR_VISITOR_OFFLINE);
}

void UIContactTreeModel::AddRootNode(QString actionType, EUINODEMODELTYPE eType)
{
    QString strUUid = QString::number((int)eType);
    QString strName = "";

    if (eType == E_UI_MODEL_QUEUE)
    {
        strName = tr("排队访客");
    }
    else if (eType == E_UI_MODEL_MONITOR_VISITOR)
    {
        strName = tr("网站访客");
    }
    else if (eType == E_UI_MODEL_MONITOR_CHAT)
    {
        strName = tr("监控对话");
    }
    else if (eType == E_UI_MODEL_COLLEAGUE)
    {
        strName = tr("我的同事");
    }
    else if (eType == E_UI_MODEL_MONITOR_VISITOR_ONLINE)
    {
        strName = tr("访问中");
    }
    else if (eType == E_UI_MODEL_MONITOR_VISITOR_OFFLINE)
    {
        strName = tr("已离开");
    }
    else
    {
        Q_ASSERT(false);
    }

    MContactItemData* data = CreateContactItemData(strUUid, strUUid, strUUid, "", "", "", eType, strName);
    if (data)
    {
        NotifyObjectChange(actionType, data);
    }
}

void UIContactTreeModel::AddColleagueGroup(const UIContactGroupInfo& info)
{
    ET("AddColleagueGroup");
    MContactItemData* data = CreateContactItemData(info.DepartmentId(), info.DepartmentId(), info.DepartmentId(), "", "", "", E_UI_MODEL_COLLEAGUE_GROUP, info.DepartmentName());
    if (data)
    {
        NotifyObjectChange("ADD_E_UI_MODEL_COLLEAGUE_GROUP", data);
    }
}

void UIContactTreeModel::AddColleagueInGroup(const UIContactInfo& info, QString departmentId)
{
    MContactItemData* data = CreateContactItemData(info.UserId(), info.UserId(), info.UserId(), "", "", departmentId, E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER, info.DisplayInnerName());
    if (data)
    {
        NotifyObjectChange("ADD_E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER", data);
    }
    MContactItemData* groupData = GetContactItemNodeById(departmentId, E_UI_MODEL_COLLEAGUE_GROUP);
    if (groupData)
    {
        int onlineNumber = groupData->GetExtendData("online").toInt();
        int allcount     = groupData->GetExtendData("allcount").toInt();
        allcount++;
        if (info.Status() == E_ONLINE || info.Status() == E_BUSY || info.Status() == E_LEAVE)
        {
            onlineNumber++;
        }
        groupData->AddExtendData("allcount", QString::number(allcount));
        groupData->AddExtendData("online", QString::number(onlineNumber));
        groupData->AddExtendData("UICONTACTTREEMODEL_CMD", "onlinecount");
        NotifyObjectChange("UPDATE_E_UI_MODEL_COLLEAGUE_GROUP", groupData);
    }
}

void UIContactTreeModel::UpdateMonitorChatColleague(const QString& customerId, bool isAdd)
{
    /*MContactItemData* colleagueData = GetContactItemNodeById( customerId, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE);
    if (colleagueData)
    {
    int nCount = colleagueData->GetExtendData("chatcount").toInt();
    if(isAdd)
    {
    nCount++;
    }
    else
    {
    if (nCount > 0)
    {
    nCount--;
    }
    else
    {
    nCount = 0;
    }

    }

    colleagueData->AddExtendData("chatcount", QString::number(nCount));
    colleagueData->AddExtendData("UICONTACTTREEMODEL_CMD", "chatcount");

    NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE",colleagueData);
    }*/
}

void UIContactTreeModel::AddMyChatInfo(const UIChatInfo& info)
{
    bool isAddChat = false;
    if (IsFindContactItemNodeById(info.CustomerID(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE))
    {
        //更新对话总数

        UIChatUserInfo* userInfo = info.GetVisitorInfo();
        if (userInfo)
        {
            QString isShow = UIPlatCommonDataModel::Instance()->GetSystemConfigByKey(ConstStringHelper::m_cNoShowVisitor, "0");
            if ((isShow != "0") || (info.VisitorMsgCount() != "0"))
            {

                if (IsFindContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
                {
                    MContactItemData* chatData = GetContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
                    if (chatData)
                    {
                        chatData->Chatid(info.ChatId());
                        chatData->AddExtendData("TokeOverID", info.TokeOverID());
                        chatData->AddExtendData("CustomerID", info.CustomerID());
                        chatData->AddExtendData("UserMsgCount", info.UserMsgCount());
                        chatData->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
                        chatData->AddExtendData("UICONTACTTREEMODEL_CMD", "msgcount");
                        NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", chatData);
                    }
                }
                else
                {
                    MContactItemData* data = BuilderContactItemData(info, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);

                    m_modelContactItemDataList.append(data);
                    m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
                    if (data)
                    {

                        data->AddExtendData("TokeOverID", info.TokeOverID());
                        data->AddExtendData("CustomerID", info.CustomerID());
                        data->AddExtendData("UserMsgCount", info.UserMsgCount());
                        data->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
                        NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", data);
                        isAddChat = true;
                    }
                }
            }
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        /*MContactItemData* data =(MContactItemData*)gUIColleagueManager->GetColleagueMemberByID(info.CustomerID(), "E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", this);
        if (data)
        {
        m_modelContactItemDataList.append(data);
        m_modelContactItemDataIndexList.insert(data->GetUUid(), data);

        */
        MContactItemData* data = CreateContactItemData(m_currentUserId, m_currentUserId, m_currentUserId, "", "", "", E_UI_MODEL_MONITOR_CHAT_COLLEAGUE, gSessionService->UserInfo()->GetDisplayName());
        if (data)
        {
            data->AddExtendData("Status", QString::number((int)gUIColleagueManager->GetPresenceStatusByID(m_currentUserId)));
            NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", data);
            UIChatUserInfo* userInfo = info.GetVisitorInfo();

            if ((userInfo == nullptr) || (IsFindContactItemNodeById(userInfo->UserId(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)))
            {
                // do nothing.
            }
            else
            {
                MContactItemData* dataChatVisitor = BuilderContactItemData(info, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);

                m_modelContactItemDataList.append(dataChatVisitor);
                m_modelContactItemDataIndexList.insert(dataChatVisitor->GetUUid(), dataChatVisitor);

                if (dataChatVisitor)
                {
                    dataChatVisitor->AddExtendData("TokeOverID", info.TokeOverID());
                    dataChatVisitor->AddExtendData("CustomerID", info.CustomerID());
                    dataChatVisitor->AddExtendData("UserMsgCount", info.UserMsgCount());
                    dataChatVisitor->AddExtendData("VisitorMsgCount", info.VisitorMsgCount());
                    NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", dataChatVisitor);
                    isAddChat = true;
                }
            }
        }
    }

    //更新对话总数
    if (isAddChat)
    {
        UpdateMonitorChatColleague(info.CustomerID());
    }
}

void UIContactTreeModel::ColleagueStateChange(QString id, EPRESENCESTATUS oldState, EPRESENCESTATUS newState, CUSTOMER_LOGIN_TYPE loginType)
{
    MContactItemData* colleagueData = GetContactItemNodeById(id, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE);
    if (colleagueData)
    {
        colleagueData->AddExtendData("oldState", QString::number(oldState));
        colleagueData->AddExtendData("newState", QString::number(newState));
        colleagueData->AddExtendData("loginType", QString::number(loginType));
        colleagueData->AddExtendData("UICONTACTTREEMODEL_CMD", "ColleagueStateChange");

        NotifyObjectChange("UPDATE_E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER", colleagueData);
        NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", colleagueData);
    }

    MContactItemData* customerData = GetContactItemNodeById(id, E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER);
    if (customerData)
    {
        customerData->AddExtendData("oldState", QString::number(oldState));
        customerData->AddExtendData("newState", QString::number(newState));
        customerData->AddExtendData("UICONTACTTREEMODEL_CMD", "ColleagueStateChange");

        NotifyObjectChange("UPDATE_E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER", customerData);
        NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", customerData);

        MContactItemData* group = GetContactItemNodeById(customerData->DepartmentId(), E_UI_MODEL_COLLEAGUE_GROUP);
        if (group)
        {
            if (oldState == E_OFFLINE)
            {
                if (newState != E_OFFLINE)
                {
                    int onlineNumber = group->GetExtendData("online").toInt();
                    onlineNumber++;
                    group->AddExtendData("online", QString::number(onlineNumber));
                    group->AddExtendData("UICONTACTTREEMODEL_CMD", "onlinecount");
                    NotifyObjectChange("UPDATE_E_UI_MODEL_COLLEAGUE_GROUP", group);
                }
            }
            else
            {
                if (newState == E_OFFLINE)
                {
                    int onlineNumber = group->GetExtendData("online").toInt();
                    onlineNumber--;
                    group->AddExtendData("online", QString::number(onlineNumber));
                    group->AddExtendData("UICONTACTTREEMODEL_CMD", "onlinecount");
                    NotifyObjectChange("UPDATE_E_UI_MODEL_COLLEAGUE_GROUP", group);
                }
            }
        }
    }
}

void UIContactTreeModel::VisitorNameChange(QString staticId, QString strNewName)
{
    if (strNewName.isEmpty())
    {
        return;
    }
    Q_FOREACH (MContactItemData* item, m_modelContactItemDataList)
    {
        if (item)
        {
            if (StringCompare(item->Userid(), staticId) && !(StringCompare(item->Name(), strNewName)))
            {
                item->AddExtendData("UICONTACTTREEMODEL_CMD", "VisitorNameChange");
                item->Name(strNewName);
                if (item->NoteType() == E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR)
                {
                    NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", item);
                }
                else if (item->NoteType() == E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR)
                {
                    NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", item);
                }
                else if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
                {

                    NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", item);
                }
                else if (item->NoteType() == E_UI_MODEL_QUEUE_VISITOR)
                {
                    NotifyObjectChange("UPDATE_E_UI_MODEL_QUEUE_VISITOR", item);
                }
                else
                {
                    // do nothing
                    Q_ASSERT(false);
                }
            }
        }
    }
}

void UIContactTreeModel::AddQueueVisitorList()
{
    QMap<QString, MContactItemData*> waitQueueList;
    gUIWaitVisitorInfoManager->GetWaitQueueList("E_UI_MODEL_QUEUE_VISITOR-QUEUEMAP", this, &waitQueueList);

    for (auto itor = waitQueueList.begin(); itor != waitQueueList.end(); itor++)
    {
        MContactItemData* info = itor.value();
        if (info)
        {
            if (IsFindContactItemNodeById(info->GetUUid(), E_UI_MODEL_QUEUE_VISITOR))
            {
                // do nothing.
            }
            else
            {
                MContactItemData* data = new MContactItemData(*info);
                if (data)
                {
                    m_modelContactItemDataList.append(data);
                    m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
                    NotifyObjectChange("ADD_E_UI_MODEL_QUEUE_VISITOR", data);
                }
            }
        }
    }
    FreeObj("E_UI_MODEL_QUEUE_VISITOR-QUEUEMAP", &waitQueueList);
}

void UIContactTreeModel::RemoveMonitorVisitorList(QStringList idList)
{
    Q_FOREACH (QString id, idList)
    {
        RemoveMonitorVisitorById(id);
    }
}

void UIContactTreeModel::DeleteMonitorVisitorList(QStringList idList)
{
    Q_FOREACH (QString id, idList)
    {
        DeleteMonitorVisitor(id);
    }
}

void UIContactTreeModel::RemoveQueueVisitorById(const QString& id)
{
    NotifyRemoveObject("REMOVE_E_UI_MODEL_QUEUE_VISITOR", id);
    RemoveModelCacheById(id, E_UI_MODEL_QUEUE_VISITOR);
}

void UIContactTreeModel::RemoveChatColleagueVisitor(QString vid)
{
    NotifyRemoveObject("REMOVE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", vid);
    RemoveModelCacheById(vid, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
}

void UIContactTreeModel::RemoveChatVisitorByIdList(QStringList idList)
{
    Q_FOREACH (QString id, idList)
    {
        MContactItemData* dataObj = GetContactItemNodeById(id, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
        if (dataObj)
        {
            RemoveChatColleagueVisitor(dataObj->GetUUid());
            UpdateMonitorChatColleague(dataObj->GetExtendData("CustomerID"), false);
        }
    }
}

void UIContactTreeModel::VisitorChatStateChange(QString vid, QString chatid, ECHATITEMSTATE newState)
{
    MContactItemData* obj = GetContactItemNodeById(vid, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
    if (obj)
    {
        obj->AddExtendData("UICONTACTTREEMODEL_CMD", "VisitorChatStateChange");
        obj->AddExtendData("newState", QString::number(newState));
        obj->Chatid(chatid);
        NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", obj);
    }
}
void UIContactTreeModel::MonitorVisitorStateChange(QString vid, EVISITORSTATUS newState, EVISITORSTATUS oldState)
{
    switch (oldState)
    {
        case E_VSTATUS_LEAVE:
        {
            switch (newState)
            {
                case E_VSTATUS_BROWSE_WEB:
                case E_VSTATUS_CONVERSATION:
                case E_VSTATUS_INVITE:
                {
                    AddMonitorVisitorById(vid);
                }
                break;
                case E_VSTATUS_LEAVE:
                    // do noting;
                    break;
                default:
                    Q_ASSERT(false);
                    break;
            }
        }
        break;
        case E_VSTATUS_BROWSE_WEB:
        case E_VSTATUS_CONVERSATION:
        case E_VSTATUS_INVITE:
        {
            switch (newState)
            {
                case E_VSTATUS_BROWSE_WEB:
                case E_VSTATUS_CONVERSATION:
                case E_VSTATUS_INVITE:
                    UpdateMonitorVisitorOnlineVisitorState(vid, newState);
                    break;
                case E_VSTATUS_LEAVE:
                    RemoveMonitorVisitorById(vid);
                    break;
                default:
                    Q_ASSERT(false);
                    break;
            }
        }

        break;
        default:

            break;
    }
}

void UIContactTreeModel::UpdateMonitorVisitorOnlineVisitorState(QString vid, EVISITORSTATUS newState)
{
    MContactItemData* obj = GetContactItemNodeById(vid, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
    if (obj)
    {
        obj->AddExtendData("UICONTACTTREEMODEL_CMD", "MonitorVisitorStateChange");
        obj->AddExtendData("NEWEVISITORSTATUS", QString::number(newState));
        NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", obj);
    }
}

void UIContactTreeModel::RemoveMonitorVisitorById(QString id)
{

    NotifyRemoveOnlineMonitorVisitorById(id, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR");
    MContactItemData* offLine = (MContactItemData*)gUIVisitorInfoManager->GetVisitorInfoById(id, "E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", this);
    if (offLine)
    {
        NotifyAddOfflineMonitorVisitor(*offLine, "ADD_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR");
        offLine->deleteLater();
    }
}

void UIContactTreeModel::AddMonitorVisitorById(QString id)
{
    MContactItemData* pinfo = (MContactItemData*)gUIVisitorInfoManager->GetVisitorInfoById(id, "E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", this);
    if (pinfo)
    {
        NotifyAddOnlineMonitorVisitor(*pinfo, "ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR");
        pinfo->deleteLater();
    }
    NotifyRemoveOfflineMonitorVisitorById(id, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR");
}

void UIContactTreeModel::AddChatList()
{
    RemoveNoChatColleagueNode();

    //////////////////////////////////////////////////////////////////////////
    QStringList chatCustomerIdList = gUIChatServiceManager->GetChatCustomerIdList();
    Q_FOREACH (QString id, chatCustomerIdList)
    {
        if (IsFindContactItemNodeById(id, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE))
        {
            MergerColleagueChatNode(id);
        }
        else
        {
            AddColleagueNodeAllChat(id);
        }
    }
}
void UIContactTreeModel::RemoveNoChatColleagueNode()
{
    QStringList chatCustomerIdList = gUIChatServiceManager->GetChatCustomerIdList();

    /////清除多余的同事节点/////////////////////////////////////////////////////////////////////
    QList<MContactItemData*> removeCustomerNodeList;
    Q_FOREACH (MContactItemData* dataObj, m_modelContactItemDataList)
    {
        if (dataObj)
        {
            if (dataObj->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE && chatCustomerIdList.indexOf(dataObj->GetUUid()) == -1)
            {
                //删除该阶节点下的所有对话
                removeCustomerNodeList.append(dataObj);
            }
        }
    }

    Q_FOREACH (MContactItemData* removeItem, removeCustomerNodeList)
    {
        if (removeItem)
        {
            QStringList idList;
            Q_FOREACH (MContactItemData* dataObj, m_modelContactItemDataList)
            {
                if (dataObj)
                {
                    if (dataObj->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR && dataObj->GetExtendData("CustomerID") == removeItem->GetUUid())
                    {
                        idList.push_back(dataObj->GetUUid());
                    }
                }
            }
            RemoveChatVisitorByIdList(idList);
            NotifyRemoveObject("REMOVE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", removeItem->GetUUid());
            RemoveModelCacheById(removeItem->GetUUid(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE);
        }
    }
}

void UIContactTreeModel::MergerColleagueChatNode(QString id)
{
    //有同事节点
    QList<MContactItemData*> chatVisitorList;
    gUIChatServiceManager->GetCustomerNewChatListByCustomerId(id, "E_UI_MODEL_MONITOR_CHAT_VISITOR—LIST", this, &chatVisitorList);
    QList<MContactItemData*> oldNodeChatList = m_modelCustomerContactItemDataIndexList.values(id);
    ////移除同事节点下，多余的对话//////////////////////////////////////////////////////////////////////
    QList<MContactItemData*> removeDataList;
    Q_FOREACH (MContactItemData* item, oldNodeChatList)
    {
        if (item == nullptr)
        {
            continue;
        }
        bool isFind = false;
        Q_FOREACH (MContactItemData* newItem, chatVisitorList)
        {
            if (newItem)
            {
                if (item->GetUUid() == newItem->GetUUid())
                {
                    isFind = true;
                    break;
                }
            }
        }
        if (!isFind)
        {
            removeDataList.append(item);
        }
    }

    Q_FOREACH (MContactItemData* removeItem, removeDataList)
    {

        RemoveChatColleagueVisitor(removeItem->GetUUid());
        UpdateMonitorChatColleague(removeItem->GetExtendData("CustomerID"), false);
    }
    //////////////////////////////////////////////////////////////////////////
    Q_FOREACH (MContactItemData* newItem, chatVisitorList)
    {
        if (newItem)
        {
            QString isShow = UIPlatCommonDataModel::Instance()->GetSystemConfigByKey(ConstStringHelper::m_cNoShowVisitor, "0");
            if ((isShow != "0") || (newItem->GetExtendData("VisitorMsgCount") != "0"))
            {
                if (IsFindContactItemNodeById(newItem->GetUUid(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
                {
                    MContactItemData* chatData = GetContactItemNodeById(newItem->GetUUid(), E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
                    if (chatData)
                    {
                        chatData->AddExtendData("TokeOverID", newItem->GetExtendData("TokeOverID"));
                        chatData->AddExtendData("CustomerID", newItem->GetExtendData("CustomerID"));
                        chatData->AddExtendData("UserMsgCount", newItem->GetExtendData("UserMsgCount"));
                        chatData->AddExtendData("VisitorMsgCount", newItem->GetExtendData("VisitorMsgCount"));
                        chatData->AddExtendData("UICONTACTTREEMODEL_CMD", "msgcount");
                        NotifyObjectChange("UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", chatData);
                    }
                }
                else
                {
                    MContactItemData* data = new MContactItemData(*newItem);
                    m_modelContactItemDataList.append(data);
                    m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
                    m_modelCustomerContactItemDataIndexList.insert(id, data);

                    NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", data);
                    UpdateMonitorChatColleague(id);
                }
            }
        }
    }
    FreeObj("E_UI_MODEL_MONITOR_CHAT_VISITOR-LIST", &chatVisitorList);
}

void UIContactTreeModel::AddColleagueNodeAllChat(QString id)
{
    MContactItemData* data = (MContactItemData*)gUIColleagueManager->GetColleagueMemberByID(id, "E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", this);
    if (data)
    {
        m_modelContactItemDataList.append(data);
        m_modelContactItemDataIndexList.insert(id, data);
        NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", data);
        QList<MContactItemData*> chatVisitorList;
        gUIChatServiceManager->GetCustomerNewChatListByCustomerId(id, "E_UI_MODEL_MONITOR_CHAT_VISITOR—LIST", this, &chatVisitorList);
        //////////////////////////////////////////////////////////////////////////
        //添加所有对话。
        Q_FOREACH (MContactItemData* itemObj, chatVisitorList)
        {
            if (itemObj)
            {
                m_modelContactItemDataList.append(itemObj);
                m_modelContactItemDataIndexList.insert(itemObj->GetUUid(), itemObj);
                m_modelCustomerContactItemDataIndexList.insert(id, itemObj);
                NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", itemObj);
                UpdateMonitorChatColleague(id);
            }
        }
        //////////////////////////////////////////////////////////////////////////
    }
    else
    {
        if (id == m_currentUserId)
        {
            //用户自己。
            MContactItemData* data = CreateContactItemData(m_currentUserId, m_currentUserId, m_currentUserId, m_currentUserId, "", "", E_UI_MODEL_MONITOR_CHAT_COLLEAGUE, gSessionService->UserInfo()->GetDisplayName());
            NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE", data);
            QList<MContactItemData*> chatVisitorList;
            gUIChatServiceManager->GetCustomerNewChatListByCustomerId(id, "E_UI_MODEL_MONITOR_CHAT_VISITOR—LIST", this, &chatVisitorList);
            //////////////////////////////////////////////////////////////////////////
            //添加所有对话。
            Q_FOREACH (MContactItemData* itemObj, chatVisitorList)
            {
                if (itemObj)
                {
                    m_modelContactItemDataList.append(itemObj);
                    m_modelContactItemDataIndexList.insert(itemObj->GetUUid(), itemObj);
                    m_modelCustomerContactItemDataIndexList.insert(id, itemObj);
                    NotifyObjectChange("ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR", itemObj);
                    UpdateMonitorChatColleague(id);
                }
            }
        }
    }
}

void UIContactTreeModel::DeleteMonitorVisitor(QString id)
{
    NotifyDeleteOnlineMonitorVisitorById(id, "DELETE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR");
    NotifyDeleteOfflineMonitorVisitorById(id, "DELETE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR");
}

void UIContactTreeModel::NotifyAddOfflineMonitorVisitor(MContactItemData& info, QString actionType)
{
    if (IsFindContactItemNodeById(info.GetUUid(), E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR))
    {
        // do nothing;
    }
    else
    {
        MContactItemData* data = new MContactItemData(info);
        m_modelContactItemDataList.append(data);
        m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
        NotifyObjectChange(actionType, data);
    }
}

void UIContactTreeModel::NotifyDeleteOfflineMonitorVisitorById(QString id, QString actionType)
{
    NotifyRemoveObject(actionType, id);
    RemoveModelCacheById(id, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);
}

void UIContactTreeModel::NotifyAddOnlineMonitorVisitor(MContactItemData& info, QString actionType)
{
    if (IsFindContactItemNodeById(info.GetUUid(), E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR))
    {
        // do nothing;
    }
    else
    {
        MContactItemData* data = new MContactItemData(info);
        m_modelContactItemDataList.append(data);
        m_modelContactItemDataIndexList.insert(data->GetUUid(), data);
        NotifyObjectChange(actionType, data);
    }
}

void UIContactTreeModel::NotifyRemoveOnlineMonitorVisitorById(QString id, QString actionType)
{
    NotifyRemoveObject(actionType, id);
    RemoveModelCacheById(id, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
}

void UIContactTreeModel::NotifyDeleteOnlineMonitorVisitorById(QString id, QString actionType)
{
    NotifyRemoveObject(actionType, id);
    RemoveModelCacheById(id, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
}

void UIContactTreeModel::NotifyRemoveOfflineMonitorVisitorById(QString id, QString actionType)
{
    NotifyRemoveObject(actionType, id);
    RemoveModelCacheById(id, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);
}

void UIContactTreeModel::R_OnContactDoubleClick(MContactItemData* item)
{
    if (item)
    {
        switch (item->NoteType())
        {
            case E_UI_MODEL_QUEUE_VISITOR:
                RemoveQueueVisitor(item->GetUUid());
                break;
            case E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR:
            {
                EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
                if (state == E_VSTATUS_BROWSE_WEB)
                {
                    UpdateMonitorVisitorOnlineVisitorState(item->Vid(), E_VSTATUS_INVITE);
                }
            }
            break;
            default:
                break;
        }
    }
}

void UIContactTreeModel::R_OnCommonTreeMenuClick(QString type, MContactItemData* item)
{
    if (StringCompare(type, ConstStringHelper::m_cMonitorVisitorChat))
    {
        if (item)
        {
            // RemoveQueueVisitor(item->GetUUid());
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cInnerContactChat))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cInviteChat))
    {
        if (item)
        {
            EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
            if (state == E_VSTATUS_BROWSE_WEB)
            {
                UpdateMonitorVisitorOnlineVisitorState(item->Vid(), E_VSTATUS_INVITE);
            }
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cDirectChat))
    {
        if (item)
        {
            EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
            if (state == E_VSTATUS_BROWSE_WEB)
            {
                UpdateMonitorVisitorOnlineVisitorState(item->Vid(), E_VSTATUS_INVITE);
            }
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

QList<MContactItemData*> UIContactTreeModel::GetContactItemNodeByCustomerId(const QString& id)
{
    QList<MContactItemData*> list = m_modelCustomerContactItemDataIndexList.values(id);
    return list;
}

void UIContactTreeModel::Lazy_RemoveCache(QString type, QString strKey, QObject* info)
{
    if (info)
    {
        MContactItemData* dataObj = (MContactItemData*)info;
        if (StringCompare(type, "DELETE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR") || StringCompare(type, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR") || StringCompare(type, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR")
            || StringCompare(type, "DELETE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR") || StringCompare(type, "REMOVE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE") || StringCompare(type, "REMOVE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR") || StringCompare(type, "REMOVE_E_UI_MODEL_QUEUE_VISITOR"))
        {
            RemoveModelCacheById(dataObj->GetUUid(), dataObj->NoteType());
        }
        else
        {
            if (type.indexOf("remove", 0, Qt::CaseInsensitive) != -1 || type.indexOf("delete", 0, Qt::CaseInsensitive) != -1)
            {
                RemoveModelCacheById(dataObj->GetUUid(), dataObj->NoteType());
                Q_ASSERT(false);
            }
        }
    }
}

void UIContactTreeModel::Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList)
{
    Q_ASSERT(false);
}

void UIContactTreeModel::R_AddChat(UIChatInfo info)
{
    AddChatInfo(info);
}

void UIContactTreeModel::R_UpdateChat(UIChatInfo info)
{
    AddChatInfo(info);
}

void UIContactTreeModel::R_DelChat(QString chatId, QString vid)
{
    MContactItemData* dataObj = GetContactItemNodeById(vid, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
    if (dataObj)
    {
        if (chatId == dataObj->Chatid())
        {
            m_modelCustomerContactItemDataIndexList.remove(dataObj->GetExtendData("CustomerID"), dataObj);
            RemoveChatColleagueVisitor(dataObj->GetUUid());
            UpdateMonitorChatColleague(dataObj->GetExtendData("CustomerID"), false);
        }
    }
}

void UIContactTreeModel::R_AddVisitor(UIVisitorInfo info)
{
    NotifyOnlineMonitorVisitor("ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", info);
    NotifyOfflineMonitorVisitor("REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", info);
}

void UIContactTreeModel::R_UpdateVisitor(UIVisitorInfo info)
{
    NotifyOnlineMonitorVisitor("ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR", info);
    NotifyOfflineMonitorVisitor("REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR", info);
}

void UIContactTreeModel::R_DelVisitor(QString uid)
{
    NotifyDeleteOnlineMonitorVisitorById(uid, "DELETE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR");
    NotifyRemoveOfflineMonitorVisitorById(uid, "DELETE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR");
}

void UIContactTreeModel::R_CreateTempChat(UIChatInfo* pInfo)
{
    if (pInfo)
    {
        AddMyChatInfo(*pInfo);
    }
}

void UIContactTreeModel::R_ShowColleagueList()
{
    AddColleagueList();
}

void UIContactTreeModel::R_GetVisitorInfoResult(QString userid, UICardInfo* info, int type)
{
    if (info)
    {
        VisitorNameChange(userid, info->Name());
    }
}

void UIContactTreeModel::R_GetWaitQueueEnd()
{
    AddQueueVisitorList();
}

void UIContactTreeModel::R_CustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType)
{
    ColleagueStateChange(fromId, oldStatus, newStatus, loginType);
}

void UIContactTreeModel::R_AddWaitQueueInfo(UIWaitVisitorInfo* pinfo)
{
    AddQueueVisitor(*pinfo);
}

void UIContactTreeModel::R_RemoveWaitQueueInfoByVid(QString fromId)
{
    RemoveQueueVisitorById(fromId);
}

void UIContactTreeModel::R_SetVisitorStatus(QString vid, EVISITORSTATUS newStatus, EVISITORSTATUS oldStatus)
{
    MonitorVisitorStateChange(vid, newStatus, oldStatus);
}

void UIContactTreeModel::R_RemoveVMVisitorList(const QStringList& list)
{
    DeleteMonitorVisitorList(list);
}

void UIContactTreeModel::R_RemoveTempVMVisitorList(const QStringList& list)
{
    RemoveMonitorVisitorList(list);
}

void UIContactTreeModel::R_CreateTransferChat(UIChatInfo* pinfo, QString msgData)
{
    AddMyChatInfo(*pinfo);
}

void UIContactTreeModel::R_AddVisitorInfo(UIVisitorInfo* pinfo)
{
    AddMonitorVisitorInfo(*pinfo);
}

void UIContactTreeModel::InitData()
{
    AddQueueRootNote();
    AddMonitorChatRootNote();
    AddColleagueRootNote();
    AddMonitorVisitorRootNote();
    NotifyObjectChange("EXPANDALL", nullptr);
}

void UIContactTreeModel::R_ReceiveChatCloseMsg(QString chatid, QString vid, QString closeType)
{
    RemoveChatInfo(vid, chatid);
}

void UIContactTreeModel::R_OnChatItemCloseClicked(QString vid, QString cid)
{
    if (IsFindContactItemNodeById(vid, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
    {
        MContactItemData* obj = GetContactItemNodeById(vid, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
        if (obj && obj->GetExtendData("CustomerID") == m_currentUserId)
        {
            RemoveChatInfo(vid, obj->Chatid());
        }
    }
}

void UIContactTreeModel::R_OnJYMainTreeMenuClick(QString type, MChatItemData* item, EUINODEMODELTYPE nodeType)
{
    if (StringCompare(type, ConstStringHelper::m_cMonitorVisitorChat))
    {
        if (item)
        {
            // RemoveQueueVisitor(item->GetUUid());
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cInnerContactChat))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cInviteChat))
    {
        if (item)
        {
            EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
            if (state == E_VSTATUS_BROWSE_WEB)
            {
                UpdateMonitorVisitorOnlineVisitorState(item->Vid(), E_VSTATUS_INVITE);
            }
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cDirectChat))
    {
        if (item)
        {
            EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
            if (state == E_VSTATUS_BROWSE_WEB)
            {
                UpdateMonitorVisitorOnlineVisitorState(item->Vid(), E_VSTATUS_INVITE);
            }
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

void UIContactTreeModel::R_OnDoubleClickChatItemData(MChatItemData* item, EUINODEMODELTYPE nodeType)
{
    if (item)
    {
        switch (nodeType)
        {
            case E_UI_MODEL_QUEUE_VISITOR:
                RemoveQueueVisitor(item->Vid());
                break;
            case E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR:
            {
                EVISITORSTATUS state = gUIVisitorInfoManager->GetVMStatus(item->Vid());
                if (state == E_VSTATUS_BROWSE_WEB)
                {
                    UpdateMonitorVisitorOnlineVisitorState(item->Vid(), E_VSTATUS_INVITE);
                }
            }
            break;
            default:
                break;
        }
    }
}

bool UIContactTreeModel::IsCheckByKey(QString type, QString strKey, QObject* info)
{
    bool isRet = false;
    if (StringCompare(type, "UPDATE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR") || StringCompare(type, "REMOVE_E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR"))
    {
        if (IsFindContactItemNodeById(strKey, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR))
        {
            MContactItemData* objTemp = GetContactItemNodeById(strKey, E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR);
            if (objTemp == info)
            {
                isRet = true;
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, "ADD_E_UI_MODEL_QUEUE_VISITOR") || StringCompare(type, "UPDATE_E_UI_MODEL_QUEUE_VISITOR"))
    {
        if (IsFindContactItemNodeById(strKey, E_UI_MODEL_QUEUE_VISITOR))
        {
            MContactItemData* objTemp = GetContactItemNodeById(strKey, E_UI_MODEL_QUEUE_VISITOR);
            if (objTemp == info)
            {
                isRet = true;
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, "ADD_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR") || StringCompare(type, "UPDATE_E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR"))
    {
        if (IsFindContactItemNodeById(strKey, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR))
        {
            MContactItemData* objTemp = GetContactItemNodeById(strKey, E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR);
            if (objTemp == info)
            {
                isRet = true;
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, "ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR") || StringCompare(type, "UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR"))
    {
        if (IsFindContactItemNodeById(strKey, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR))
        {
            MContactItemData* objTemp = GetContactItemNodeById(strKey, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR);
            if (objTemp == info)
            {
                isRet = true;
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, "UPDATE_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE") || StringCompare(type, "ADD_E_UI_MODEL_MONITOR_CHAT_COLLEAGUE"))
    {
        if (IsFindContactItemNodeById(strKey, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE))
        {
            MContactItemData* objTemp = GetContactItemNodeById(strKey, E_UI_MODEL_MONITOR_CHAT_COLLEAGUE);
            if (objTemp == info)
            {
                isRet = true;
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, "ADD_E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER") || StringCompare(type, "UPDATE_E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER"))
    {
        if (IsFindContactItemNodeById(strKey, E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER))
        {
            MContactItemData* objTemp = GetContactItemNodeById(strKey, E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER);
            if (objTemp == info)
            {
                isRet = true;
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, "ADD_E_UI_MODEL_COLLEAGUE_GROUP"))
    {
        if (IsFindContactItemNodeById(strKey, E_UI_MODEL_COLLEAGUE_GROUP))
        {
            MContactItemData* objTemp = GetContactItemNodeById(strKey, E_UI_MODEL_COLLEAGUE_GROUP);
            if (objTemp == info)
            {
                isRet = true;
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, "EXPANDALL"))
    {
        isRet = true;
    }
    else
    {
        Q_ASSERT(false);
    }
    return isRet;
}
