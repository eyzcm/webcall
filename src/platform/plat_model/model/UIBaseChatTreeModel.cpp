#include "stdafx.h"
#include "UIBaseChatTreeModel.h"
#include "utils/ConstStringHelper.h"
#include "utils/DefineHelper.h"
#include "model/uiplatcommondatamodel.h"
#include "UIContactTreeModel.h"

#include "serviceinterface/dbuiplatservice.h"


//#include "protocol_biz/webcallprotocolservice.h"
#include "session/el_sessionservice.h"

#include "service/PlatCommonSignalService.h"

UIBaseChatTreeModel::UIBaseChatTreeModel(void)
{
    m_currentUserId    = "";
    m_currChatItemData = nullptr;
}

UIBaseChatTreeModel::~UIBaseChatTreeModel(void)
{
    qDeleteAll(m_modelList);
    m_modelIndexList.clear();
}

void UIBaseChatTreeModel::Init()
{
    connect(this, SIGNAL(S_RemoveChatItem(QString)), PlatCommonSignalService::Instance(), SIGNAL(S_RemoveChatItem(QString)));


    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnRemoveChatItemData(MChatItemData*)), this, SLOT(R_OnRemoveChatItemData(MChatItemData*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OnChatItemCloseClickedEnd(QString, QString)), this, SLOT(R_OnChatItemCloseClickedEnd(QString, QString)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CloseChat(MChatItemData*)), this, SLOT(R_CloseChat(MChatItemData*)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_AddChat(UIChatInfo)), this, SLOT(R_AddChat(UIChatInfo)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_UpdateChat(UIChatInfo)), this, SLOT(R_UpdateChat(UIChatInfo)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_DelChat(QString, QString)), this, SLOT(R_DelChat(QString, QString)));
    //////////////////////////////////////////////////////////////////////////
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CreateTempChat(UIChatInfo*)), this, SLOT(R_CreateTempChat(UIChatInfo*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_ShowNotice(CommInfo*)), this, SLOT(R_ShowNotice(CommInfo*)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_GetVisitorInfoResult(QString, UICardInfo*, int)), this, SLOT(R_GetVisitorInfoResult(QString, UICardInfo*, int)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_ReceiveMessage(const Im_Message&)), this, SLOT(R_ReceiveMessage(const Im_Message&)));

    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CustomerPresenceChanged(QString, EPRESENCESTATUS, EPRESENCESTATUS, CUSTOMER_LOGIN_TYPE)), this, SLOT(R_CustomerPresenceChanged(QString, EPRESENCESTATUS, EPRESENCESTATUS, CUSTOMER_LOGIN_TYPE)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_OtherSettingChanged()), this, SLOT(R_OtherSettingChanged()));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_CreateTransferChat(UIChatInfo*, QString)), this, SLOT(R_CreateTransferChat(UIChatInfo*, QString)));
    connect(PlatCommonSignalService::Instance(), SIGNAL(S_ReceiveChatCloseMsg(QString, QString, QString)), this, SLOT(R_ReceiveChatCloseMsg(QString, QString, QString)));
    //////////////////////////////////////////////////////////////////////////
}

void UIBaseChatTreeModel::NotifyCreateChatItem(MChatItemData* item)
{

    // addzcm 处理野指针问题 ，暂时改为同步方式
    NotifyObjectChange("ADD_CHATITEM", item);
    // AsyncNotifyObjectChange("ADD_CHATITEM", item->Vid(), item);
}

void UIBaseChatTreeModel::NotifyRomoveChatItem(QString vid)
{
    NotifyRemoveObject("REMOVE_CHATITEM", vid);
}

void UIBaseChatTreeModel::NotifyModifyChatItem(MChatItemData* item)
{
    NotifyObjectChange("MODIFY_CHATITEM", item);
}

void UIBaseChatTreeModel::NotifyModifyChatItem(QString type, MChatItemData* item)
{
    NotifyObjectChange(type, item);
}

void UIBaseChatTreeModel::NotifyAddChatItem(const UIChatInfo& obj, QString type, bool isSelect)
{
    QString isShow = gUISystemConfigManager->GetSystemConfigByKey(ConstStringHelper::m_cNoShowVisitor, "0");

    if (isShow == "0")
    {
        if (obj.GetVisitorInfo())
        {
            if (StringCompare(type, "chat"))
            {

                if (m_modelIndexList.find(obj.GetVisitorInfo()->UserId()) != m_modelIndexList.end())
                {

                    MChatItemData* mdata = m_modelIndexList[obj.GetVisitorInfo()->UserId()];

                    if ((mdata) && (mdata->ChatState() != E_CHATITEM_INVITE))
                    {

                        //不显示未说话的访客
                        /*
                        bool isHistoryMessage = gUIHistoryManager->HasRecordHistoryByChatId(obj.ChatId(), obj.GetVisitorInfo()->UserId());

                        if (!obj.TempChat() && !isHistoryMessage)
                        {
                            m_chatIdList.push_back(obj.ChatId());
                            return;
                        }
                        */
                    }
                }
                else
                {
                    bool isHistoryMessage = this->HasRecordHistoryByChatId(obj.ChatId(), obj.GetVisitorInfo()->UserId());

                    if (!obj.TempChat() && !isHistoryMessage)
                    {
                        m_chatIdList.push_back(obj.ChatId());
                        return;
                    }
                }
            }
        }
    }

    if (obj.GetVisitorInfo())
    {
        if (IsFindChatItemNodeById(obj.GetVisitorInfo()->UserId()))
        {
            //更新节点
            MChatItemData* item = GetMChatItemDataById(obj.GetVisitorInfo()->UserId());
            if (item)
            {
                item->Chatid(obj.ChatId());
                item->AddExtendData("CustomerID", obj.CustomerID());
                item->AddExtendData("TokeOverID", obj.TokeOverID());
                item->AddExtendData("CustomerName", obj.CustomerName());
                if (item->ChatState() == E_CHATITEM_BROWSE)
                {
                    if (obj.TempChat())
                    {
                        VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_INVITE);
                    }
                    else
                    {
                        if (obj.CustomerID() == m_currentUserId)
                        {
                            VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_CHAT);
                        }
                        else
                        {
                            VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_MONITOR_CHAT);
                        }
                    }
                }
                else
                {
                    if (obj.TempChat())
                    {
                        VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_INVITE);
                    }
                    else
                    {
                        if (StringCompare(type, "closechat"))
                        {

                            if (obj.CustomerID() == m_currentUserId)
                            {
                                VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_CHAT_CLOSE);
                            }
                            else
                            {
                                VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_MONITOR_CHAT_CLOSE);
                            }
                        }
                        else if (StringCompare(type, "chat") || StringCompare(type, "monitor"))
                        {
                            if (obj.CustomerID() == m_currentUserId)
                            {
                                item->Name(gUICacheService->GetDisplayName(&obj));
                                VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_CHAT);
                            }
                            else
                            {
                                VisitorChatStateChange(item->Vid(), obj.ChatId(), E_CHATITEM_MONITOR_CHAT);
                            }
                        }
                        else if (StringCompare(type, "DoReceiveVisitorMessage"))
                        {
                        }
                        else
                        {
                            Q_ASSERT(false);
                        }
                    }
                }
                if (isSelect)
                {
                    NotifySelectChatItem(item);
                }
                else
                {
                    if (obj.Flash())
                        NotifyTwinkleChatItem(item);
                }
            }
        }
        else
        {
            //创建节点
            MChatItemData* item = CreateMChatItemData(obj, type);
            NotifyCreateChatItem(item);
            if (isSelect)
            {
                NotifySelectChatItem(item);
            }
            else
            {
                if (obj.Flash())
                    NotifyTwinkleChatItem(item);
            }
        }
    }
}

void UIBaseChatTreeModel::NotifyAddChatItem(const UIContactInfo& obj, bool isSelect)
{
    if (IsFindChatItemNodeById(obj.UserId()))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj.UserId());
        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
    else
    {
        //创建节点
        MChatItemData* item = UIBaseFactory::CreateMChatItemData(obj);
        AddCache(item);

        NotifyCreateChatItem(item);
        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
}

void UIBaseChatTreeModel::NotifyAddChatItem(const UIVisitorInfo& obj, bool isSelect)
{
    if (IsFindChatItemNodeById(obj.Vid()))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj.Vid());

        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
    else
    {
        //创建节点
        MChatItemData* item = UIBaseFactory::CreateMChatItemData(obj);
        AddCache(item);
        NotifyCreateChatItem(item);
        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
}

void UIBaseChatTreeModel::NotifyAddChatItem(const UIWaitVisitorInfo& obj, bool isSelect)
{
    if (IsFindChatItemNodeById(obj.UserId()))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj.UserId());
        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
    else
    {
        //创建节点
        MChatItemData* item = UIBaseFactory::CreateMChatItemData(obj);
        AddCache(item);
        item->AddExtendData("ip", obj.Visitor_ip());
        NotifyCreateChatItem(item);
        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
}

MChatItemData* UIBaseChatTreeModel::CreateMChatItemData(const UIChatInfo& obj, QString type)
{
    ECHATITEMSTATE state = GetNodeStateByChatInfo(obj, type);
    MChatItemData* ret   = UIBaseFactory::CreateMChatItemData(obj, state);
    AddCache(ret);
    return ret;
}

MChatItemData* UIBaseChatTreeModel::CreateMChatItemData(const CommInfo& obj)
{
    MChatItemData* ret = nullptr;
    ret                = new MChatItemData();
    ret->InitData(obj.GetValueByKey("noticeId"), obj.GetValueByKey("noticeId"), "", obj.GetValueByKey("subject"), E_NOTICE, E_CHATITEM_SETUP);
    ret->AddExtendData("url", obj.GetValueByKey("url"));
    AddCache(ret);
    return ret;
}

void UIBaseChatTreeModel::SetCurrentUserId(QString curUserId)
{
    m_currentUserId = curUserId;
}

bool UIBaseChatTreeModel::IsFindChatItemNodeById(const QString& id)
{
    if (m_modelIndexList.find(id) != m_modelIndexList.end())
    {
        return true;
    }
    return false;
}

MChatItemData* UIBaseChatTreeModel::GetMChatItemDataById(const QString& id)
{
    MChatItemData* ret = nullptr;
    if (m_modelIndexList.find(id) != m_modelIndexList.end())
    {
        ret = m_modelIndexList[id];
    }
    return ret;
}

ECHATITEMSTATE UIBaseChatTreeModel::GetNodeStateByChatInfo(const UIChatInfo& obj, QString type)
{
    ECHATITEMSTATE stat = E_CHATITEM_CHAT;
    if (StringCompare(obj.CustomerID(), m_currentUserId))
    {
        if (StringCompare(type, "chat"))
        {
            if (obj.TempChat())
            {
                stat = E_CHATITEM_INVITE;
            }
            else
            {
                stat = E_CHATITEM_CHAT;
            }
        }
        else if (StringCompare(type, "invite"))
        {
            stat = E_CHATITEM_INVITE;
        }
        else if (StringCompare(type, "closechat"))
        {
            stat = E_CHATITEM_CHAT_CLOSE;
        }
        else
        {
            Q_ASSERT(false);
        }
    }
    else
    {
        if (StringCompare(type, "closechat"))
        {
            stat = E_CHATITEM_MONITOR_CHAT_CLOSE;
        }
        else
        {
            stat = E_CHATITEM_MONITOR_CHAT;
        }
    }
    return stat;
}

void UIBaseChatTreeModel::NotifyAddChatItem(const CommInfo& obj, bool isSelect)
{
    if (IsFindChatItemNodeById(obj.GetValueByKey("noticeId")))
    {
        //更新节点
        MChatItemData* item = GetMChatItemDataById(obj.GetValueByKey("noticeId"));
        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
    else
    {
        //创建节点
        MChatItemData* item = CreateMChatItemData(obj);

        NotifyCreateChatItem(item);
        if (isSelect)
        {
            NotifySelectChatItem(item);
        }
        else
        {
            NotifyTwinkleChatItem(item);
        }
    }
}

void UIBaseChatTreeModel::RemoveChatItemById(const QString& id)
{
    Q_EMIT S_RemoveChatItem(id);

    NotifyRomoveChatItem(id);
    RemoveCache(id);
}

void UIBaseChatTreeModel::RemoveCache(const MChatItemData* pdata)
{
    foreach (MChatItemData* pinfo, m_modelList)
    {
        if ((pinfo == pdata) && (pinfo))
        {
            RemoveCache(pinfo->Vid());
            break;
        }
    }
}

void UIBaseChatTreeModel::RemoveCache(const QString& id)
{
    if (m_currChatItemData && m_currChatItemData->Vid() == id)
    {
        m_currChatItemData = nullptr;
    }
    auto fItor = m_modelIndexList.find(id);
    if (fItor != m_modelIndexList.end())
    {
        MChatItemData* item = fItor.value();
        //删除列表索引。
        m_modelList.removeOne(item);
        if (item)
        {
            item->deleteLater();
            item = nullptr;
        }
        m_modelIndexList.erase(fItor);
    }
}

void UIBaseChatTreeModel::RemoveChatItem(MChatItemData* item)
{
    if (item)
    {
        RemoveChatItemById(item->Vid());
    }
}

void UIBaseChatTreeModel::NotifySelectChatItem(MChatItemData* item)
{
    m_currChatItemData = item;
    NotifyObjectChange("SELECT_CHATITEM", item);
}

void UIBaseChatTreeModel::NotifyTwinkleChatItem(MChatItemData* item)
{
    NotifyObjectChange("TWINKLE_CHATITEM", item);
}

void UIBaseChatTreeModel::NotifyTwinkleChatItem(QString chatid)
{
    if (m_currChatItemData && m_currChatItemData->Chatid() == chatid)
    {
        return;
    }
    if (IsFindChatItemNodeByChatId(chatid))
    {
        MChatItemData* item = GetMChatItemDataByChatId(chatid);
        if (item)
        {
            NotifyTwinkleChatItem(item);
        }
    }
}

void UIBaseChatTreeModel::ColleagueStateChange(QString id, EPRESENCESTATUS oldState, EPRESENCESTATUS newState, CUSTOMER_LOGIN_TYPE loginType)
{
    MChatItemData* obj = GetMChatItemDataById(id);
    if (obj)
    {
        ColleagueStateChanged(oldState, newState, loginType, obj);
    }
}

void UIBaseChatTreeModel::VisitorNameChange(QString staticId, QString strNewName)
{
    if (strNewName.isEmpty())
    {
        return;
    }
    Q_FOREACH (MChatItemData* item, m_modelList)
    {
        if (item)
        {
            if (StringCompare(item->Userid(), staticId) && !(StringCompare(item->Name(), strNewName)))
            {
                item->AddExtendData("UICHATTREEMODEL_CMD", "VisitorNameChange");
                item->Name(strNewName);
                NotifyModifyChatItem(item);
            }
        }
    }
}

QList<MChatItemData*> UIBaseChatTreeModel::GetMChatItemDataListByStaticId(const QString& staticId)
{
    QList<MChatItemData*> retList;
    Q_FOREACH (MChatItemData* item, m_modelList)
    {
        if (item)
        {
            if (StringCompare(item->Userid(), staticId))
            {
                retList.push_back(item);
            }
        }
    }
    return retList;
}

void UIBaseChatTreeModel::VisitorChatStateChange(QString vid, QString chatid, ECHATITEMSTATE newState)
{
    MChatItemData* obj = GetMChatItemDataById(vid);
    if (obj)
    {
        obj->Chatid(chatid);
        NotifyChatStateChanged(obj, newState);
    }
}

void UIBaseChatTreeModel::R_OnChatItemSelectChanged(MChatItemData* oldObj, MChatItemData* currObj)
{
    SetCurrentChatItemData(currObj);
}

MChatItemData* UIBaseChatTreeModel::GetCurrentChatItemData() const
{
    return m_currChatItemData;
}

void UIBaseChatTreeModel::SetCurrentChatItemData(MChatItemData* val)
{
    m_currChatItemData = val;
}

void UIBaseChatTreeModel::NotifyReceiveMessage(QString chatid)
{
    if (IsFindChatItemNodeByChatId(chatid))
    {
        MChatItemData* item = GetMChatItemDataByChatId(chatid);
        if (item)
        {

            if (item->ChatState() == E_CHATITEM_MONITOR_CHAT_CLOSE)
            {
                MChatItemData* newitem = new MChatItemData(*item);
                newitem->AddExtendData("UICHATTREEMODEL_CMD", "VisitorChatStateChange");
                newitem->AddExtendData("oldState", QString::number(item->ChatState()));
                newitem->AddExtendData("newState", QString::number(E_CHATITEM_MONITOR_CHAT));
                newitem->ChatState(E_CHATITEM_MONITOR_CHAT);

                NotifyModifyChatItem(newitem);
                item->ChatState(E_CHATITEM_MONITOR_CHAT);
            }
            else if (item->ChatState() == E_CHATITEM_CHAT_CLOSE)
            {
                MChatItemData* newitem = new MChatItemData(*item);
                newitem->AddExtendData("UICHATTREEMODEL_CMD", "VisitorChatStateChange");
                newitem->AddExtendData("oldState", QString::number(item->ChatState()));
                newitem->AddExtendData("newState", QString::number(E_CHATITEM_CHAT));
                newitem->ChatState(E_CHATITEM_CHAT);

                NotifyModifyChatItem(newitem);

                item->ChatState(E_CHATITEM_CHAT);
            }

            NotifyObjectChange("RECEIVEMESSAGE_CHATITEM", item);
        }
    }
    else
    {
        if (m_chatIdList.indexOf(chatid) != -1)
        {
            UIChatInfo* info = nullptr;
            bool        isOk = false;
            do
            {
                info = UIPlatCommonDataModel::Instance()->GetChatByChatId(chatid);
                if (info)
                {
                    isOk = true;
                    break;
                }
                info = UIPlatCommonDataModel::Instance()->GetChatInfoByChatId(chatid);
                if (info)
                {
                    isOk = true;
                    break;
                }
            } while (false);
            if (!isOk)
            {
                return;
            }
            MChatItemData* objData = BuilderChatItemData(info);
            if (objData)
            {
                //添加。
                //创建节点
                AddCache(objData);
                NotifyCreateChatItem(objData);
                NotifyTwinkleChatItem(objData);

                NotifyObjectChange("RECEIVEMESSAGE_CHATITEM", objData);
            }
        }
    }
}

bool UIBaseChatTreeModel::IsFindChatItemNodeByChatId(const QString& id)
{
    bool ret = false;
    Q_FOREACH (MChatItemData* item, m_modelList)
    {
        if (item)
        {
            if (item->Chatid() == id)
            {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

MChatItemData* UIBaseChatTreeModel::GetMChatItemDataByChatId(const QString& id)
{
    MChatItemData* ret = nullptr;
    Q_FOREACH (MChatItemData* item, m_modelList)
    {
        if (item)
        {
            if (item->Chatid() == id)
            {
                ret = item;
                break;
            }
        }
    }
    return ret;
}

void UIBaseChatTreeModel::DoClearAllCloseChatItem()
{

    //todo 9999  增加扩展
    ESUMMARIZEONCLOSE summarizeType =E_NO_FORCE;// gUIPersonalConfigManager->SummarizeStatus();
    //删除所有关闭的对话
    bool isRemoveCurrentItem = IsCheckRemoveCloseChat(m_currChatItemData, summarizeType);

    QList<MChatItemData*> removeList;
    Q_FOREACH (MChatItemData* itemObj, m_modelList)
    {
        bool isChecked = IsCheckRemoveCloseChat(itemObj, summarizeType);
        if (isChecked)
        {
            removeList.push_back(itemObj);
        }
    }
    Q_FOREACH (MChatItemData* item, removeList)
    {
        RemoveChatItem(item);
    }

    if (isRemoveCurrentItem)
    {
        if (m_modelList.size() > 0)
        {
            NotifySelectChatItem(m_modelList.at(0));
        }
    }
}

void UIBaseChatTreeModel::R_OnRemoveChatItemData(MChatItemData* obj)
{
    DoRemoveChatItemData(obj);
}

void UIBaseChatTreeModel::R_OnChatItemCloseClickedEnd(QString vid, QString cid)
{
    DoChatItemCloseClicked(vid, cid);
}

void UIBaseChatTreeModel::NotifyRobotAutoReplayError(MChatItemData* objData)
{
    if (objData == nullptr)
    {
        return;
    }
    MChatItemData* obj = GetMChatItemDataById(objData->Vid());
    if (obj)
    {
        obj->AddExtendData("UICHATTREEMODEL_CMD", "NotifyRobotAutoReplayError");
        NotifyObjectChange("MODIFY_CHATITEM", obj);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void UIBaseChatTreeModel::RemoveChatVisitorByIdList(QStringList idList)
{
    Q_FOREACH (QString id, idList)
    {
        MChatItemData* dataObj = GetMChatItemDataById(id);
        if (dataObj)
        {
            VisitorChatStateChange(dataObj->Vid(), dataObj->Chatid(), E_CHATITEM_CHAT_CLOSE);
        }
    }
}

void UIBaseChatTreeModel::NotifyChatStateChanged(MChatItemData* obj, ECHATITEMSTATE newState)
{
    if (obj)
    {
        switch (newState)
        {
            case E_CHATITEM_BROWSE:
                // do nothing.
                break;

            case E_CHATITEM_CHAT_CLOSE:
            case E_CHATITEM_CHAT:
            case E_CHATITEM_INVITE:
                if (obj->ChatState() != newState)
                {
                    obj->AddExtendData("UICHATTREEMODEL_CMD", "VisitorChatStateChange");
                    obj->AddExtendData("oldState", QString::number(obj->ChatState()));
                    obj->AddExtendData("newState", QString::number(newState));
                    obj->ChatState(newState);
                    obj->UserType(E_VISITOR);
                    NotifyModifyChatItem(obj);
                }
                break;
            case E_CHATITEM_MONITOR_CHAT:
            case E_CHATITEM_MONITOR_CHAT_CLOSE:
                if (obj->ChatState() != newState)
                {
                    obj->AddExtendData("UICHATTREEMODEL_CMD", "VisitorChatStateChange");
                    obj->AddExtendData("oldState", QString::number(obj->ChatState()));
                    obj->AddExtendData("newState", QString::number(newState));
                    obj->ChatState(newState);
                    obj->UserType(E_MONITOR);
                    NotifyModifyChatItem(obj);
                }
                break;
        }
    }
}

void UIBaseChatTreeModel::NotifyChatStateChanged(QList<MChatItemData*> removeChatItemList)
{
    Q_FOREACH (MChatItemData* dataObj, removeChatItemList)
    {
        if (dataObj)
        {
            if (dataObj->UserType() == E_VISITOR)
            {
                NotifyChatStateChanged(dataObj, E_CHATITEM_CHAT_CLOSE);
            }
            else if (dataObj->UserType() == E_MONITOR)
            {
                NotifyChatStateChanged(dataObj, E_CHATITEM_MONITOR_CHAT_CLOSE);
            }
            else
            {
                Q_ASSERT(false);
            }
        }
    }
}

void UIBaseChatTreeModel::ColleagueStateChanged(EPRESENCESTATUS oldState, EPRESENCESTATUS newState, CUSTOMER_LOGIN_TYPE loginType, MChatItemData* obj)
{
    if (obj)
    {
        do
        {
            if (oldState == newState)
            {
                break;
            }
            obj->AddExtendData("UICHATTREEMODEL_CMD", "ColleagueStateChange");
            obj->AddExtendData("oldState", QString::number(oldState));
            obj->AddExtendData("newState", QString::number(newState));
            obj->AddExtendData("loginType", QString::number(loginType));

            NotifyModifyChatItem(obj);
        } while (false);
    }
}

void UIBaseChatTreeModel::DoCloseChat(QString vid, QString chatId)
{
    bool isFind = false;
    if (IsFindChatItemNodeById(vid))
    {

        MChatItemData* objData = GetMChatItemDataById(vid);
        if (objData->Chatid() == chatId)
        {
            isFind = true;
            CloseChat(objData);
        }
    }

    if (!isFind)
    {
        if (IsFindChatItemNodeByChatId(chatId))
        {
            MChatItemData* objData = GetMChatItemDataByChatId(chatId);
            CloseChat(objData);
        }
    }
}

void UIBaseChatTreeModel::AddCache(MChatItemData* item)
{
    if (item)
    {
        AddCacheToList(item);
        m_modelIndexList.insert(item->Vid(), item);
    }
}

bool UIBaseChatTreeModel::IsCheckRemoveCloseChat(MChatItemData* itemObj, ESUMMARIZEONCLOSE summarizeType)
{
    bool isCheckRemove = false;
    if (itemObj)
    {
        if (itemObj->ChatState() == E_CHATITEM_INVITE || itemObj->ChatState() == E_CHATITEM_LEAVE || itemObj->ChatState() == E_CHATITEM_MONITOR_CHAT_CLOSE)
        {
            isCheckRemove = true;
        }
        else if (itemObj->ChatState() == E_CHATITEM_CHAT_CLOSE && summarizeType == E_NO_FORCE)
        {
            isCheckRemove = true;
        }
        else
        {
            // do nothing;
        }
    }
    return isCheckRemove;
}

void UIBaseChatTreeModel::Lazy_RemoveCache(QString type, QString strKey, QObject* info)
{
    if (info)
    {
        MChatItemData* objData = (MChatItemData*)info;
        if (type == "REMOVE_CHATITEM")
        {
            if (objData)
            {
                RemoveCache(objData);
            }
        }
        else
        {
            if (type.indexOf("remove", 0, Qt::CaseInsensitive) != -1 || type.indexOf("delete", 0, Qt::CaseInsensitive) != -1)
            {
                RemoveCache(objData);
                Q_ASSERT(false);
            }
        }
    }
}

void UIBaseChatTreeModel::Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList)
{

    Q_ASSERT(false);
}

void UIBaseChatTreeModel::R_AddChat(UIChatInfo info)
{
    DoAddChatInfo(info);
}

void UIBaseChatTreeModel::R_UpdateChat(UIChatInfo info)
{
    DoUpdateChatInfo(info);
}

void UIBaseChatTreeModel::R_DelChat(QString chatID, QString vid)
{

    if (IsFindChatItemNodeByChatId(chatID))
    {
        MChatItemData* objData = GetMChatItemDataByChatId(chatID);
        if (objData)
        {
            QString customid = objData->GetExtendData("CustomerID");
            if (customid != gSessionService->UserInfo()->UserId())
                DoDeleteChatInfo(chatID, vid);
        }
    }
}

void UIBaseChatTreeModel::CloseChat(MChatItemData* objData)
{
    if (objData)
    {
        if (objData->UserType() == E_VISITOR)
        {
            NotifyChatStateChanged(objData, E_CHATITEM_CHAT_CLOSE);
        }
        else if (objData->UserType() == E_MONITOR)
        {
            NotifyChatStateChanged(objData, E_CHATITEM_MONITOR_CHAT_CLOSE);
        }
        else
        {
            Q_ASSERT(false);
            // do nothing
        }
    }
}

void UIBaseChatTreeModel::OtherSettingChanged()
{
    bool    isShow        = true;
    QString noShowVisitor = gUISystemConfigManager->GetSystemConfigByKey(ConstStringHelper::m_cNoShowVisitor, "0");
    //不显示未说话的访客
    if (noShowVisitor == "0")
    {
        isShow = false;
    }
    if (isShow)
    {
        //显示
        Q_FOREACH (QString chatId, m_chatIdList)
        {
            UIChatInfo* info = nullptr;
            bool        isOk = false;
            do
            {
                info = UIPlatCommonDataModel::Instance()->GetChatByChatId(chatId);
                if (info)
                {
                    isOk = true;
                    break;
                }
                info = UIPlatCommonDataModel::Instance()->GetChatInfoByChatId(chatId);
                if (info)
                {
                    isOk = true;
                    break;
                }
            } while (false);
            if (!isOk)
            {
                continue;
            }
            MChatItemData* objData = BuilderChatItemData(info);
            if (objData)
            {

                if (IsFindChatItemNodeById(objData->Vid()))
                {
                    //更新。
                    MChatItemData* objItem = GetMChatItemDataById(objData->Vid());
                    if (objItem)
                    {
                        objItem->Chatid(objData->Chatid());
                        objItem->AddExtendData("CustomerID", objData->GetExtendData("CustomerID"));
                        objItem->AddExtendData("CustomerName", objData->GetExtendData("CustomerName"));
                        objItem->AddExtendData("TokeOverID", objData->GetExtendData("TokeOverID"));
                    }
                    VisitorChatStateChange(objItem->Vid(), objItem->Chatid(), objData->ChatState());
                }
                else
                {
                    //添加。
                    //创建节点
                    AddCache(objData);
                    NotifyCreateChatItem(objData);
                    NotifyTwinkleChatItem(objData);
                }
            }
        }
        m_chatIdList.clear();
    }
    else
    {
        //不显示
        Q_FOREACH (MChatItemData* obj, m_modelList)
        {
            if (obj->ChatState() == E_CHATITEM_CHAT && obj->UserType() == E_VISITOR)
            {
                
                bool isHistoryMessage = gUIHistoryManager->HasRecordHistoryByChatId(obj->Chatid(), obj->Vid());
                if (!isHistoryMessage)
                {
                    m_chatIdList.push_back(obj->Chatid());
                    RemoveChatItemById(obj->Vid());
                }
                
            }
        }
    }
}

void UIBaseChatTreeModel::R_CreateTempChat(UIChatInfo* pInfo)
{
    DoCreateTempChat(pInfo);
}

void UIBaseChatTreeModel::R_ShowNotice(CommInfo* pInfo)
{
    DoShowNotice(pInfo);
}

void UIBaseChatTreeModel::R_GetVisitorInfoResult(QString userid, UICardInfo* info, int type)
{
    DoGetVisitorInfoResult(userid, info, type);
}

void UIBaseChatTreeModel::R_ReceiveMessage(const Im_Message& msg)
{
    DoReceiveMessage(msg);
}

void UIBaseChatTreeModel::R_OtherSettingChanged()
{
    DoOtherSettingChanged();
}

void UIBaseChatTreeModel::R_CreateTransferChat(UIChatInfo* pinfo, QString msgData)
{
    DoCreateTransferChat(pinfo, msgData);
}

void UIBaseChatTreeModel::R_CustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType)
{
    DoCustomerPresenceChanged(fromId, newStatus, oldStatus, loginType);
}

void UIBaseChatTreeModel::R_ReceiveChatCloseMsg(QString chatid, QString vid, QString closeType)
{
    DoReceiveChatCloseMsg(chatid, vid, closeType);
}

void UIBaseChatTreeModel::R_CloseChat(MChatItemData* dataObj)
{
    DoRobotCloseChat(dataObj);
}

void UIBaseChatTreeModel::DoRemoveChatItemData(MChatItemData* dataObj)
{
    if (dataObj)
    {
        RemoveCache(dataObj->Vid());
    }
}

void UIBaseChatTreeModel::DoChatItemCloseClicked(QString vid, QString chatId)
{
    if (IsFindChatItemNodeById(vid))
    {
        MChatItemData* obj = GetMChatItemDataById(vid);
        if (obj)
        {
            bool isUpdateSelect = false;
            if (m_currChatItemData && m_currChatItemData->Vid() == vid)
            {
                isUpdateSelect = true;
            }
            if (obj->Chatid() != chatId)
            {
                Q_ASSERT(false);
            }
            NotifyRomoveChatItem(vid);
            RemoveCache(obj->Vid());
            if (isUpdateSelect)
            {
                if (m_modelList.size() > 0)
                {
                    NotifySelectChatItem(m_modelList.at(0));
                }
            }
        }
    }
}

void UIBaseChatTreeModel::DoRobotCloseChat(MChatItemData* dataObj)
{
    CloseChat(dataObj);
}

void UIBaseChatTreeModel::DoAddChatInfo(UIChatInfo info)
{
    if (info.CustomerID() == m_currentUserId)
    {
        NotifyAddChatItem(info, "chat");
    }
    else
    {
        if (info.IsWatch(m_currentUserId))
        {
            NotifyAddChatItem(info, "monitor");
        }
    }
}

void UIBaseChatTreeModel::DoUpdateChatInfo(UIChatInfo info)
{
    if (info.CustomerID() == m_currentUserId)
    {
        NotifyAddChatItem(info, "chat");
    }
    else
    {
        if (info.IsWatch(m_currentUserId))
        {
            NotifyAddChatItem(info, "monitor");
        }
    }
}

void UIBaseChatTreeModel::DoDeleteChatInfo(QString chatId, QString vid)
{
    if (IsFindChatItemNodeByChatId(chatId))
    {
        MChatItemData* objData = GetMChatItemDataByChatId(chatId);
        CloseChat(objData);
    }
}

void UIBaseChatTreeModel::DoCreateTempChat(UIChatInfo* pInfo)
{
    NotifyAddChatItem(*pInfo, "chat");
}

void UIBaseChatTreeModel::DoShowNotice(CommInfo* pInfo)
{
    NotifyAddChatItem(*pInfo, false);
}

void UIBaseChatTreeModel::DoGetVisitorInfoResult(QString userid, UICardInfo* info, int type)
{
    if (info)
    {
        VisitorNameChange(userid, info->Name());
    }
}

void UIBaseChatTreeModel::DoReceiveMessage(const Im_Message& msg)
{
    if (msg.GetValueByKey("colleagueMessage") == "0")
    {

        if (msg.GetMsgType() == E_MSGTYPE_MESSAGE || msg.GetMsgType() == E_MSGTYPE_SYSMESSAGE || msg.GetMsgType() == E_MSGTYPE_SYSMESSAGE_WELCOME || msg.GetMsgType() == E_MSGTYPE_FILE || msg.GetMsgType() == E_MSGTYPE_COMPOSING || msg.GetMsgType() == E_MSGTYPE_SCREENSHOTS
            || msg.GetMsgType() == E_MSGTYPE_WXVOICE || msg.GetMsgType() == E_MSGTYPE_VIDEO || msg.GetMsgType() == E_MSGTYPE_VOICE || msg.GetMsgType() == E_MSGTYPE_IMAGE)
       {
            QString chatId = msg.GetValueByKey("chatId");
            NotifyReceiveMessage(chatId);
            NotifyTwinkleChatItem(chatId);
        }
    }
    else if (msg.GetValueByKey("colleagueMessage") == "1")
    {
        //同事消息
        QString fromId = msg.GetValueByKey("fromUserId");
        if (fromId.isEmpty())
        {
            fromId = msg.GetValueByKey("from");
        }
        UIContactInfo* member = UIPlatCommonDataModel::Instance()->GetColleagueMemberByID(fromId);
        if (member)
        {
            NotifyAddChatItem(member, false);
        }
        if (msg.GetMsgType() == E_MSGTYPE_MESSAGE || msg.GetMsgType() == E_MSGTYPE_SYSMESSAGE || msg.GetMsgType() == E_MSGTYPE_SYSMESSAGE_WELCOME || msg.GetMsgType() == E_MSGTYPE_FILE || msg.GetMsgType() == E_MSGTYPE_SCREENSHOTS || msg.GetMsgType() == E_MSGTYPE_WXVOICE
            || msg.GetMsgType() == E_MSGTYPE_VIDEO || msg.GetMsgType() == E_MSGTYPE_VOICE || msg.GetMsgType() == E_MSGTYPE_IMAGE)
         {
            NotifyReceiveMessage(fromId);
            NotifyTwinkleChatItem(fromId);
        }
    }
    else
    {
    }
}

void UIBaseChatTreeModel::DoCustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType)
{
    ColleagueStateChange(fromId, oldStatus, newStatus, loginType);
}

void UIBaseChatTreeModel::DoOtherSettingChanged()
{
    OtherSettingChanged();
}

void UIBaseChatTreeModel::DoCreateTransferChat(UIChatInfo* pinfo, QString msgData)
{
    NotifyAddChatItem(*pinfo, "chat");
}

void UIBaseChatTreeModel::DoReceiveChatCloseMsg(QString chatid, QString vid, QString closeType)
{
    DoCloseChat(vid, chatid);
}

void UIBaseChatTreeModel::AddCacheToList(MChatItemData* itemObj)
{
    m_modelList.append(itemObj);
}

void UIBaseChatTreeModel::NotifyRobotTakeOver(MChatItemData* objData)
{
    if (objData == nullptr)
    {
        return;
    }
    MChatItemData* obj = GetMChatItemDataById(objData->Vid());
    if (obj)
    {
        obj->TakeOver(true);
        obj->AddExtendData("UICHATTREEMODEL_CMD", "NotifyRobotTakeOver");
        NotifyModifyChatItem(obj);
    }
}

bool UIBaseChatTreeModel::IsCheckByKey(QString type, QString strKey, QObject* info)
{
    bool isRet = false;
    if (StringCompare(type, "ADD_CHATITEM") || StringCompare(type, "SELECT_CHATITEM") || StringCompare(type, "TWINKLE_CHATITEM") || StringCompare(type, "RECEIVEMESSAGE_CHATITEM"))
    {
        if (IsFindChatItemNodeById(strKey))
        {
            MChatItemData* objTemp = GetMChatItemDataById(strKey);
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
    else
    {
        Q_ASSERT(false);
    }
    return isRet;
}

bool UIBaseChatTreeModel::HasRecordHistoryByChatId(const QString &chatid, const QString &vid)
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

