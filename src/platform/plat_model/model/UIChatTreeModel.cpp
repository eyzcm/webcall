#include "stdafx.h"
#include "UIChatTreeModel.h"
#include "utils/ConstStringHelper.h"
#include "utils/DefineHelper.h"
#include "model/uiplatcommondatamodel.h"
#include "UIContactTreeModel.h"
#include "service/platcommonmodelservice.h"

#include "model/uiplatcommondatamodel.h"
#include "service/PlatCommonSignalService.h"


UIChatTreeModel::UIChatTreeModel(void)
{
}

UIChatTreeModel::~UIChatTreeModel(void)
{
}

void UIChatTreeModel::Init()
{
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnClickChatItemData(MChatItemData*, EUINODEMODELTYPE)), this, SLOT(R_OnClickChatItemData(MChatItemData*, EUINODEMODELTYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnBeginDoubleClickChatItemData(MChatItemData*, EUINODEMODELTYPE)), this, SLOT(R_OnBeginDoubleClickChatItemData(MChatItemData*, EUINODEMODELTYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnJYMainTreeMenuClick(QString, MChatItemData*, EUINODEMODELTYPE)), this, SLOT(R_OnJYMainTreeMenuClick(QString, MChatItemData*, EUINODEMODELTYPE)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnContactSingleClick(MContactItemData*)), this, SLOT(R_OnContactSingleClick(MContactItemData*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnContactDoubleClick(MContactItemData*)), this, SLOT(R_OnContactDoubleClick(MContactItemData*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnCommonTreeMenuClick(QString, MContactItemData*)), this, SLOT(R_OnCommonTreeMenuClick(QString, MContactItemData*)));
    UIBaseChatTreeModel::Init();
}

void UIChatTreeModel::R_OnContactSingleClick(MContactItemData* item)
{
    if (item == nullptr)
    {
        return;
    }
    if (IsFindChatItemNodeById(item->GetUUid()))
    {
        MChatItemData* objData = GetMChatItemDataById(item->GetUUid());
        if (objData)
        {
            objData->Chatid(item->Chatid());

            if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
            {
                if (objData->ChatState() == E_CHATITEM_BROWSE)
                {
                    objData->UserType(E_MONITOR);
                    VisitorChatStateChange(objData->Vid(), objData->Chatid(), E_CHATITEM_MONITOR_CHAT);
                }
                else
                {
                    NotifySelectChatItem(objData);
                }
            }
            else
            {
                NotifySelectChatItem(objData);
            }
        }
    }
    else
    {
        switch (item->NoteType())
        {
            case E_UI_MODEL_QUEUE_VISITOR:
                AddQueueVisitor(item);
                break;
            case E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR:
                AddChatVisitor(item);
                break;
            case E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR:
                AddMonitorVisitor(item);
                break;
            case E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR:
                AddMonitorVisitor(item);
                break;
            default:
                break;
        }
    }
}

void UIChatTreeModel::R_OnContactDoubleClick(MContactItemData* item)
{
    if (item == nullptr)
    {
        return;
    }
    if (IsFindChatItemNodeById(item->GetUUid()))
    {
        MChatItemData* objData = GetMChatItemDataById(item->GetUUid());
        if (objData)
        {
            objData->Chatid(item->Chatid());
            if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
            {
                if (objData->ChatState() == E_CHATITEM_BROWSE)
                {
                    objData->UserType(E_MONITOR);
                    VisitorChatStateChange(objData->Vid(), objData->Chatid(), E_CHATITEM_MONITOR_CHAT);
                }
                else
                {
                    NotifySelectChatItem(objData);
                }
            }
            else
            {
                NotifySelectChatItem(objData);
            }
        }
    }
    else
    {
        switch (item->NoteType())
        {
            case E_UI_MODEL_MONITOR_CHAT_COLLEAGUE:
            case E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER:
                AddContactInnerChat(item);
                break;
            default:
                break;
        }
    }
}

void UIChatTreeModel::R_OnClickChatItemData(MChatItemData* item, EUINODEMODELTYPE nodeType)
{
    if (item == nullptr)
    {
        return;
    }
    if (IsFindChatItemNodeById(item->Vid()))
    {
        MChatItemData* objData = GetMChatItemDataById(item->Vid());
        if (objData)
        {
            objData->Chatid(item->Chatid());

            if (nodeType == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
            {
                if (objData->ChatState() == E_CHATITEM_BROWSE)
                {
                    objData->UserType(E_MONITOR);
                    VisitorChatStateChange(objData->Vid(), objData->Chatid(), E_CHATITEM_MONITOR_CHAT);
                }
                else
                {
                    NotifySelectChatItem(objData);
                }
            }
            else
            {
                NotifySelectChatItem(objData);
            }
        }
    }
    else
    {
        switch (nodeType)
        {
            case E_UI_MODEL_QUEUE_VISITOR:
                AddQueueVisitor(item);
                break;
            case E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR:
                AddChatVisitor(item);
                break;
            case E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR:
                AddMonitorVisitor(item);
                break;
            case E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR:
                AddMonitorVisitor(item);
                break;
            default:
                break;
        }
    }
}

void UIChatTreeModel::R_OnBeginDoubleClickChatItemData(MChatItemData* item, EUINODEMODELTYPE nodeType)
{

    if (item == nullptr)
    {
        return;
    }
    if (IsFindChatItemNodeById(item->Vid()))
    {
        MChatItemData* objData = GetMChatItemDataById(item->Vid());
        if (objData)
        {
            objData->Chatid(item->Chatid());
            if (nodeType == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
            {
                if (objData->ChatState() == E_CHATITEM_BROWSE)
                {
                    objData->UserType(E_MONITOR);
                    VisitorChatStateChange(objData->Vid(), objData->Chatid(), E_CHATITEM_MONITOR_CHAT);
                }
                else
                {
                    NotifySelectChatItem(objData);
                }
            }
            else
            {
                NotifySelectChatItem(objData);
            }
        }
    }
    else
    {
        switch (nodeType)
        {
            case E_UI_MODEL_MONITOR_CHAT_COLLEAGUE:
            case E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER:
                AddContactInnerChat(item);
                break;
            default:
                break;
        }
    }
}


void UIChatTreeModel::R_OnCommonTreeMenuClick(QString type, MContactItemData* item)
{
    if (StringCompare(type, ConstStringHelper::m_cMonitorVisitorChat))
    {
        if (item)
        {
            if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
            {
                UpdateChatItem(item);
            }
            else if (item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE)
            {
                QList<MContactItemData*> list = UIContactTreeModel::GetInstance()->GetContactItemNodeByCustomerId(item->GetUUid());
                Q_FOREACH (MContactItemData* itemObj, list)
                {
                    UpdateChatItem(itemObj);
                }
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
    else if (StringCompare(type, ConstStringHelper::m_cInnerContactChat))
    {
        AddContactInnerChat(item);
    }
    else if (StringCompare(type, ConstStringHelper::m_cInviteChat))
    {
    }
    else if (StringCompare(type, ConstStringHelper::m_cDirectChat))
    {
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

void UIChatTreeModel::AddContactInnerChat(MContactItemData* item)
{
    if (item == nullptr)
    {
        return;
    }
    if (IsFindChatItemNodeById(item->GetUUid()))
    {
        //更新节点
        MChatItemData* chatObj = GetMChatItemDataById(item->GetUUid());
        NotifySelectChatItem(chatObj);
    }
    else
    {
        //创建节点
        MChatItemData* chatObj = CreateMChatItemData(*item, E_CUSTOMER, E_CHATITEM_CHAT);
        NotifyCreateChatItem(chatObj);
        NotifySelectChatItem(chatObj);
    }
}

void UIChatTreeModel::AddQueueVisitor(MContactItemData* obj)
{
    if (obj == nullptr)
    {
        return;
    }

    if (IsFindChatItemNodeById(obj->Vid()))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj->Vid());
        NotifySelectChatItem(item);
    }
    else
    {
        //创建节点
        MChatItemData* item = CreateMChatItemData(*obj, E_VISITOR, E_CHATITEM_BROWSE);
        NotifyCreateChatItem(item);
        NotifySelectChatItem(item);
    }
}

void UIChatTreeModel::AddChatVisitor(MContactItemData* item)
{
    if (item == nullptr)
    {
        return;
    }

    if (IsFindChatItemNodeById(item->GetUUid()))
    {
        MChatItemData* itemObj = GetMChatItemDataById(item->GetUUid());
        if (itemObj)
        {
            //选中该节点。
            NotifySelectChatItem(itemObj);
        }
    }
    else
    {
        //添加节点
        if (item->GetExtendData("CustomerID") == m_currentUserId)
        {
            if (item->GetExtendData("TempChat") == "1")
            {
                MChatItemData* itemObj = CreateMChatItemData(*item, E_VISITOR, E_CHATITEM_INVITE);
                if (itemObj)
                {
                    NotifyCreateChatItem(itemObj);
                    NotifySelectChatItem(itemObj);
                }
            }
            else
            {
                MChatItemData* itemObj = CreateMChatItemData(*item, E_VISITOR, E_CHATITEM_CHAT);
                if (itemObj)
                {
                    itemObj->AddExtendData("ip", item->GetExtendData("ip"));
                    NotifyCreateChatItem(itemObj);
                    NotifySelectChatItem(itemObj);
                }
            }
            //自己的对话
        }
        else
        {
            //监控对话
            QString customerId   = item->GetExtendData("CustomerID");
            QString customerName = item->GetExtendData("CustomerName");
            QString strName      = "";
            if (!customerName.isEmpty())
            {
                strName = QString(tr("监控室%1%2")).arg(customerName).arg(item->Chatid().right(4));
            }
            else
            {
                strName = QString(tr("监控室%1%2")).arg(customerId).arg(item->Chatid().right(4));
            }

            MChatItemData* itemObj = CreateMChatItemData(*item, E_MONITOR, E_CHATITEM_MONITOR_CHAT);
            if (itemObj)
            {
                itemObj->Name(strName);
                NotifyCreateChatItem(itemObj);
                NotifySelectChatItem(itemObj);
            }
        }
    }
}

void UIChatTreeModel::AddMonitorVisitor(MContactItemData* obj)
{
    if (obj == nullptr)
    {
        return;
    }

    if (IsFindChatItemNodeById(obj->Vid()))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj->Vid());
        NotifySelectChatItem(item);
    }
    else
    {
        //创建节点
        MChatItemData* item = CreateMChatItemData(*obj, E_VISITOR, E_CHATITEM_BROWSE);
        item->AddExtendData("ip", obj->GetExtendData("ip"));
        NotifyCreateChatItem(item);
        NotifySelectChatItem(item);
    }
}

MChatItemData* UIChatTreeModel::CreateMChatItemData(const MContactItemData& obj, EUSERTYPE eType, ECHATITEMSTATE eState)
{
    MChatItemData* ret = nullptr;
    ret                = new MChatItemData();
    ret->InitData(obj.Vid(), obj.Userid(), obj.Chatid(), obj.Name(), eType, eState);
    ret->ExtendInfo(obj.ExtendInfo());
    AddCache(ret);
    return ret;
}

void UIChatTreeModel::UpdateChatItem(MContactItemData* item)
{
    if (item && item->NoteType() == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
    {
        if (IsFindChatItemNodeById(item->GetUUid()))
        {
            MChatItemData* objData = GetMChatItemDataById(item->GetUUid());
            if (objData)
            {
                if (objData->ChatState() == E_CHATITEM_BROWSE)
                {
                    objData->UserType(E_MONITOR);
                    VisitorChatStateChange(objData->Vid(), objData->Chatid(), E_CHATITEM_MONITOR_CHAT);
                }
                else
                {
                    NotifySelectChatItem(objData);
                }
            }
        }
        else
        {
            AddChatVisitor(item);
        }
    }
}

void UIChatTreeModel::AddQueueVisitor(MChatItemData* obj)
{
    if (obj == nullptr)
    {
        return;
    }

    if (IsFindChatItemNodeById(obj->Vid()))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj->Vid());
        NotifySelectChatItem(item);
    }
    else
    {
        //创建节点
        MChatItemData* item = new MChatItemData(*obj);
        item->ChatState(E_CHATITEM_BROWSE);
        AddCache(item);
        NotifyCreateChatItem(item);
        NotifySelectChatItem(item);
    }
}

void UIChatTreeModel::AddChatVisitor(MChatItemData* item)
{
    if (item == nullptr)
    {
        return;
    }

    if (IsFindChatItemNodeById(item->Vid()))
    {
        MChatItemData* itemObj = GetMChatItemDataById(item->Vid());
        if (itemObj)
        {
            //选中该节点。
            NotifySelectChatItem(itemObj);
        }
    }
    else
    {
        //添加节点
        if (item->GetExtendData("CustomerID") == m_currentUserId)
        {
            if (item->GetExtendData("TempChat") == "1")
            {
                MChatItemData* itemObj = new MChatItemData(*item);
                itemObj->ChatState(E_CHATITEM_INVITE);
                AddCache(itemObj);
                if (itemObj)
                {
                    NotifyCreateChatItem(itemObj);
                    NotifySelectChatItem(itemObj);
                }
            }
            else
            {
                MChatItemData* itemObj = new MChatItemData(*item);
                itemObj->ChatState(E_CHATITEM_CHAT);
                AddCache(itemObj);
                if (itemObj)
                {
                    itemObj->AddExtendData("ip", item->GetExtendData("ip"));
                    NotifyCreateChatItem(itemObj);
                    NotifySelectChatItem(itemObj);
                }
            }
            //自己的对话
        }
        else
        {
            //监控对话
            QString customerId   = item->GetExtendData("CustomerID");
            QString customerName = item->GetExtendData("CustomerName");
            QString strName      = "";
            if (!customerName.isEmpty())
            {
                strName = QString(tr("监控室%1%2")).arg(customerName).arg(item->Chatid().right(4));
            }
            else
            {
                strName = QString(tr("监控室%1%2")).arg(customerId).arg(item->Chatid().right(4));
            }

            MChatItemData* itemObj = new MChatItemData(*item);
            itemObj->ChatState(E_CHATITEM_MONITOR_CHAT);
            itemObj->UserType(E_MONITOR);
            AddCache(itemObj);
            if (itemObj)
            {
                itemObj->Name(strName);
                NotifyCreateChatItem(itemObj);
                NotifySelectChatItem(itemObj);
            }
        }
    }
}

void UIChatTreeModel::AddMonitorVisitor(MChatItemData* obj)
{
    if (obj == nullptr)
    {
        return;
    }

    if (IsFindChatItemNodeById(obj->Vid()))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj->Vid());
        NotifySelectChatItem(item);
    }
    else
    {
        //创建节点
        MChatItemData* item = new MChatItemData(*obj);
        item->ChatState(E_CHATITEM_BROWSE);
        AddCache(item);
        item->AddExtendData("ip", obj->GetExtendData("ip"));
        NotifyCreateChatItem(item);
        NotifySelectChatItem(item);
    }
}

void UIChatTreeModel::AddContactInnerChat(MChatItemData* item)
{
    if (item == nullptr)
    {
        return;
    }
    if (IsFindChatItemNodeById(item->Vid()))
    {
        //更新节点
        MChatItemData* chatObj = GetMChatItemDataById(item->Vid());
        NotifySelectChatItem(chatObj);
    }
    else
    {
        //创建节点
        MChatItemData* chatObj = new MChatItemData(*item);
        chatObj->UserType(E_CUSTOMER);
        chatObj->ChatState(E_CHATITEM_CHAT);
        AddCache(item);
        NotifyCreateChatItem(chatObj);
        NotifySelectChatItem(chatObj);
    }
}

void UIChatTreeModel::UpdateChatItem(MChatItemData* item, EUINODEMODELTYPE noteType)
{
    if (item && noteType == E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR)
    {
        if (IsFindChatItemNodeById(item->Vid()))
        {
            MChatItemData* objData = GetMChatItemDataById(item->Vid());
            if (objData)
            {
                if (objData->ChatState() == E_CHATITEM_BROWSE)
                {
                    objData->UserType(E_MONITOR);
                    VisitorChatStateChange(objData->Vid(), objData->Chatid(), E_CHATITEM_MONITOR_CHAT);
                }
                else
                {
                    NotifySelectChatItem(objData);
                }
            }
        }
        else
        {
            AddChatVisitor(item);
        }
    }
}
