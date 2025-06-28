#include "UICommonDataModel.h"
#include "utils/ConstStringHelper.h"
#include "control/CommonSignalService.h"
#include "service/DbUiwebcallService.h"

UIBaseCommonDataModel::UIBaseCommonDataModel()
{
}

UIBaseCommonDataModel::~UIBaseCommonDataModel()
{
}

void UIBaseCommonDataModel::Init()
{
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OtherSettingChanged()), this, SLOT(R_OtherSettingChanged()));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_MessageTipSettingChanged()), this, SLOT(R_MessageTipSettingChanged()));

    connect(CommonSignalService::GetInstance(), SIGNAL(S_UpdateChat(UIChatInfo)), this, SLOT(R_UpdateChat(UIChatInfo)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_DelChat(QString, QString)), this, SLOT(R_DelChat(QString, QString)));

    connect(CommonSignalService::GetInstance(), SIGNAL(S_UpdateVisitor(UIVisitorInfo)), this, SLOT(R_UpdateVisitor(UIVisitorInfo)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_DelVisitor(QString)), this, SLOT(R_DelVisitor(QString)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_CreateTempChat(UIChatInfo*)), this, SLOT(R_CreateTempChat(UIChatInfo*)));
}

//判断是否存在该标签（曹振超 2019年8月5日）
bool UIBaseCommonDataModel::FindSummaryTagByName(QString strName)
{
    return gUIOtherServiceManager->findSummaryTagByName(strName);
}

//获取/清理 访客来源类型
QMap<QString, EngineData*> UIBaseCommonDataModel::GetEngineMap()
{
    QList<EngineData*> retList;
    gUISystemConfigManager->GetEngineList("ENGINEDATA-LIST", this, &retList);

    QMap<QString, EngineData*> mapEngine = QMap<QString, EngineData*>();
    for (int i = 0; i < retList.size(); ++i)
    {
        mapEngine.insert(retList.at(i)->m_id, retList.at(i));
    }
    return mapEngine;
}
void UIBaseCommonDataModel::ClearEngineMap(QMap<QString, EngineData*>& map)
{
    this->FreeObj("ENGINEDATA-LIST", &map);
}

//获取/清理 访客来源渠道
QMap<QString, UIChannelInfo*> UIBaseCommonDataModel::GetChannelMap()
{
    QMap<QString, UIChannelInfo*> mapChannel;
    gUIOtherServiceManager->GetChannel(ConstStringHelper::m_cUICHANNELINFO_MAP, this, &mapChannel);
    return mapChannel;
}
void UIBaseCommonDataModel::ClearChannelMap(QMap<QString, UIChannelInfo*>& map)
{
    this->FreeObj(ConstStringHelper::m_cUICHANNELINFO_MAP, &map);
}

//获取/清理 访客子站点
QMap<QString, UISiteInfo*> UIBaseCommonDataModel::GetSubsiteMap()
{
    QMap<QString, UISiteInfo*> mapSubsite;
    gUIOtherServiceManager->GetSite(ConstStringHelper::m_cUISITEINFO_MAP, this, &mapSubsite);
    return mapSubsite;
}
void UIBaseCommonDataModel::ClearChannelMap(QMap<QString, UISiteInfo*>& map)
{
    this->FreeObj(ConstStringHelper::m_cUISITEINFO_MAP, &map);
}

//获取/设置 访客信息
UIVisitorFilterInfo* UIBaseCommonDataModel::GetUIVisitorFilterInfo()
{
    UIVisitorFilterInfo* info = gUIPersonalConfigYLService->VisitorFilterInfo();

    return info;
}
void UIBaseCommonDataModel::SetUIVisitorFilterInfo(UIVisitorFilterInfo* info)
{
    if (info)
    {
        gUIPersonalConfigYLService->VisitorFilterInfo(info);
    }
}

UISystemTipData* UIBaseCommonDataModel::GetSystemTipByKey(QString type)
{
    UISystemTipData* ret = GetSystemTipCaCheByKey(type);
    do
    {
        if (ret)
        {
            break;
        }

        UISystemTipData* customTip = (UISystemTipData*)gUIPersonalConfigManager->GetSystemTipByKey(type, ConstStringHelper::m_cUISYSTEMTIPDATA, this);
        if (customTip)
        {
            m_systemTipMapList.insert(type, customTip);
            ret = customTip;
            break;
        }

        UISystemTipData* defaultTip = (UISystemTipData*)gUISystemConfigManager->GetSystemTipByKey(type, ConstStringHelper::m_cUISYSTEMTIPDATA, this);
        if (defaultTip)
        {
            m_systemTipMapList.insert(type, defaultTip);
            ret = defaultTip;
            break;
        }

    } while (false);
    return ret;
}

UISystemTipData* UIBaseCommonDataModel::GetSystemTipCaCheByKey(QString strKey)
{

    QMutexLocker     locker(&m_mutex);
    UISystemTipData* ret = nullptr;
    if (m_systemTipMapList.find(strKey) != m_systemTipMapList.end())
    {
        ret = m_systemTipMapList[strKey];
    }
    return ret;
}

QList<UIHistoryInfo*> UIBaseCommonDataModel::GetHistoryListByChatId(QString chatid)
{
    QList<UIHistoryInfo*> retList;
    gUIHistoryManager->GetHistoryListByChatId(chatid, "UIHISTORYINFO-LIST", this, &retList);
    return retList;
}

QList<CTabItemInfo*> UIBaseCommonDataModel::GetTabInfoList()
{
    QList<CTabItemInfo*> retList;
    gUIPersonalConfigManager->GetTabInfoList("CTABITEMINFO-LIST", this, &retList);
    return retList;
}

void UIBaseCommonDataModel::ClearTabInfoList(QList<CTabItemInfo*>& list)
{
    this->FreeObj("CTABITEMINFO-LIST", &list);
}

CommInfo* UIBaseCommonDataModel::GetIconInfoByKey(const QString& key)
{
    CommInfo* info = (CommInfo*)gUIPersonalConfigManager->GetIconInfoByKey(key, ConstStringHelper::m_cCOMMINFO, this);
    return info;
}

CommInfo* UIBaseCommonDataModel::GetTopMenuByKey(const QString& key)
{
    CommInfo* info = (CommInfo*)gUIPersonalConfigManager->GetTopMenuInfoByKey(key, ConstStringHelper::m_cCOMMINFO, this);
    return info;
}

void UIBaseCommonDataModel::ClearHistoryList(QList<UIHistoryInfo*>& list)
{
    this->FreeObj("UIHISTORYINFO-LIST", &list);
}

UIChatInfo* UIBaseCommonDataModel::GetChatByChatId(QString id, int isNoUiData)
{
    QMutexLocker locker(&m_mutex);
    UIChatInfo*  info = nullptr;
    do
    {
        if (isNoUiData == 0)
        {
            if (m_chatInfoMapList.find(id) != m_chatInfoMapList.end())
            {
                info = m_chatInfoMapList[id];
                break;
            }
        }
        info = (UIChatInfo*)gUIChatServiceManager->GetChatByChatId(id, ConstStringHelper::m_cUICHATINFO, this);
        if (info)
        {
            if (m_chatInfoMapList.find(id) != m_chatInfoMapList.end())
            {
                UIChatInfo* tempInfo = m_chatInfoMapList[id];
                if (tempInfo)
                {
                    tempInfo->deleteLater();
                    tempInfo = nullptr;
                }
            }
            m_chatInfoMapList.insert(id, info);
        }
        else
        {
            info = (UIChatInfo*)gUIChatRecordServiceManager->findChatRecordById(id, ConstStringHelper::m_cUICHATINFO, this);
            if (m_chatInfoMapList.find(id) != m_chatInfoMapList.end())
            {
                UIChatInfo* tempInfo = m_chatInfoMapList[id];
                if (tempInfo)
                {
                    tempInfo->deleteLater();
                    tempInfo = nullptr;
                }
            }
            m_chatInfoMapList.insert(id, info);
        }

    } while (false);

    return info;
}

UIVisitorInfo* UIBaseCommonDataModel::GetVisitorInfoById(QString id)
{
    UIVisitorInfo* visitor = nullptr;
    do
    {
        if (m_visitorInfoMapList.find(id) != m_visitorInfoMapList.end())
        {
            visitor = m_visitorInfoMapList[id];
            break;
        }
        visitor = (UIVisitorInfo*)gUIVisitorInfoManager->GetVisitorInfoById(id, ConstStringHelper::m_cUIVISITORINFO, this);
        if (visitor)
        {
            m_visitorInfoMapList.insert(id, visitor);
        }

    } while (false);

    return visitor;
}

UIChatInfo* UIBaseCommonDataModel::GetChatByVid(QString id)
{
    QMutexLocker locker(&m_mutex);
    UIChatInfo*  info = nullptr;
    do
    {
        info = (UIChatInfo*)gUIConversationManager->GetChatByVid(id, ConstStringHelper::m_cUICHATINFO, this);
        if (info)
        {
            if (m_chatInfoMapList.find(info->ChatId()) != m_chatInfoMapList.end())
            {
                UIChatInfo* tempInfo = m_chatInfoMapList[info->ChatId()];
                if (tempInfo)
                {
                    tempInfo->deleteLater();
                    tempInfo = nullptr;
                }
            }
            m_chatInfoMapList.insert(info->ChatId(), info);
        }
    } while (false);

    return info;
}

void UIBaseCommonDataModel::UpdateChat(UIChatInfo* info)
{
    if (info)
    {
        if (m_chatInfoMapList.contains(info->ChatId()))
        {
            m_chatInfoMapList[info->ChatId()]->AiChatFlag(info->AiChatFlag());
        }
    }
}

UIContactInfo* UIBaseCommonDataModel::GetColleagueMemberByID(QString id)
{

    UIContactInfo* info = nullptr;
    do
    {
        if (m_contactInfoMapList.find(id) != m_contactInfoMapList.end())
        {
            info = m_contactInfoMapList[id];
        }
        if (info)
        {
            break;
        }
        info = (UIContactInfo*)gUIColleagueManager->GetColleagueMemberByID(id, ConstStringHelper::m_cUICONTACTINFO, this);
        if (info)
        {
            m_contactInfoMapList.insert(id, info);
        }
    } while (false);
    return info;
}

UIContactInfo* UIBaseCommonDataModel::GetColleagueMemberByIDIgnoreExist(QString id)
{

    UIContactInfo* info = nullptr;
    do
    {
        if (m_contactInfoMapList.find(id) != m_contactInfoMapList.end())
        {
            info = m_contactInfoMapList[id];
        }
        if (info)
        {
            break;
        }
        info = (UIContactInfo*)gUIColleagueManager->GetColleagueMemberByID(id, ConstStringHelper::m_cUICONTACTINFO, this);
        if (info == nullptr)
        {
            //好友列表
            info = (UIContactInfo*)gUIColleagueManager->GetFriendByID(id, ConstStringHelper::m_cUICONTACTINFO, this);
        }

        if (info)
        {
            m_contactInfoMapList.insert(id, info);
        }
        else
        {
            if ((!id.isEmpty()) && (id.length() < 20))
            {
                if (m_contactInfoTempMapList.find(id) != m_contactInfoTempMapList.end())
                {
                    info = m_contactInfoTempMapList[id];
                }
                if (info)
                {
                    break;
                }
                info = new UIContactInfo();
                info->UserId(id);
                info->NickName(id);
                m_contactInfoTempMapList.insert(id, info);
            }
        }

    } while (false);
    return info;
}

UITrackInfo* UIBaseCommonDataModel::GetCurrentTrackByVid(QString id)
{
    UITrackInfo* track = (UITrackInfo*)gUITrackInfoManager->GetCurrentTrackByVid(id, ConstStringHelper::m_cUITRACKINFO, this);
    return track;
}

QList<UISystemTipData*> UIBaseCommonDataModel::DefaultSystemTipList()
{
    QList<UISystemTipData*> retList;
    gUISystemConfigManager->DefaultSystemTipList("UISYSTEMTIPDATA-LIST", this, &retList);
    return retList;
}

QList<UISystemTipData*> UIBaseCommonDataModel::CustomSystemTipList()
{
    QList<UISystemTipData*> retList;
    gUIPersonalConfigManager->CustomSystemTipList("UISYSTEMTIPDATA-LIST", this, &retList);
    return retList;
}

QList<UIExtSystemTipData*> UIBaseCommonDataModel::CustomExtSystemTipList()
{
    QList<UIExtSystemTipData*> retList;
    gUIPersonalConfigManager->CustomExtSystemTipList("UIEXTSYSTEMTIPDATA-LIST", this, &retList);
    return retList;
}

void UIBaseCommonDataModel::ClearSystemTipList(QList<UISystemTipData*>& list)
{
    this->FreeObj("UISYSTEMTIPDATA-LIST", &list);
}

void UIBaseCommonDataModel::ClearExtSystemTipList(QList<UIExtSystemTipData*>& list)
{
    this->FreeObj("UIEXTSYSTEMTIPDATA-LIST", &list);
}

UICardInfo* UIBaseCommonDataModel::GetVisitorCardByUserId(QString id)
{
    UICardInfo* info = nullptr;
    info             = (UICardInfo*)gUIVisitorInfoManager->GetVisitorCardByUserId(id, ConstStringHelper::m_cUICARDINFO, this);
    return info;
}

UIWaitVisitorInfo* UIBaseCommonDataModel::GetWaitQueueInfoById(QString id)
{
    UIWaitVisitorInfo* info = nullptr;
    info                    = (UIWaitVisitorInfo*)gUIWaitVisitorInfoManager->GetWaitQueueInfoById(id, ConstStringHelper::m_cUIWAITVISITORINFO, this);
    return info;
}

CExtendInfo* UIBaseCommonDataModel::GetExtendInfoByKey(QString key)
{
    CExtendInfo* info = nullptr;
    info              = (CExtendInfo*)gUIExtendConfigManager->GetExtendInfoByKey(key, ConstStringHelper::m_cEXTENDORDERINFO, this);
    return info;
}

QList<UITrackInfo*> UIBaseCommonDataModel::GetTrackInfoByVid(QString id)
{
    QList<UITrackInfo*> retList;
    gUITrackInfoManager->GetTrackInfoByVid(id, "UITRACKINFO-LIST", this, &retList);
    return retList;
}

void UIBaseCommonDataModel::ClearTrackInfoList(QList<UITrackInfo*>& list)
{
    this->FreeObj("UITRACKINFO-LIST", &list);
}

QList<ExtendOrderInfo*> UIBaseCommonDataModel::ExtendOrderInfoList()
{
    QList<ExtendOrderInfo*> retList;
    gUIExtendConfigManager->ExtendOrderInfoList("EXTENDORDERINFO-LIST", this, &retList);
    return retList;
}

void UIBaseCommonDataModel::ClearExtendOrderInfoList(QList<ExtendOrderInfo*>& list)
{
    this->FreeObj("EXTENDORDERINFO-LIST", &list);
}

QList<MContactItemData*> UIBaseCommonDataModel::GetColleagueMemberList()
{
    QList<MContactItemData*> memberList;
    gUIColleagueManager->GetColleagueMemberList("E_UI_MODEL_COLLEAGUE_CUSTOMER-LIST", this, &memberList);
    return memberList;
}

QList<MContactItemData*> UIBaseCommonDataModel::GetCommonwordUser()
{
    QList<MContactItemData*> memberList;
    gUIColleagueManager->GetCommonwordUser("E_UI_MODEL_COLLEAGUE_CUSTOMER-LIST", this, &memberList);
    return memberList;
}
void UIBaseCommonDataModel::ClearColleagueMemberList(QList<MContactItemData*>& list)
{
    this->FreeObj("E_UI_MODEL_COLLEAGUE_CUSTOMER-LIST", &list);
}

QList<MContactItemData*> UIBaseCommonDataModel::GetColleagueMemberListByGroupID(QString id)
{
    QList<MContactItemData*> memberList;
    gUIColleagueManager->GetColleagueMemberListByGroupID(id, "E_UI_MODEL_COLLEAGUE_CUSTOMER-LIST", this, &memberList);
    return memberList;
}

QMap<QString, MContactItemData*> UIBaseCommonDataModel::GetColleagueGroupList()
{
    QMap<QString, MContactItemData*> grouplist;
    gUIColleagueManager->GetColleagueGroupList("E_UI_MODEL_COLLEAGUE_GROUP-GROUPMAP", this, &grouplist);
    return grouplist;
}

void UIBaseCommonDataModel::ClearColleagueGroupList(QMap<QString, MContactItemData*>& list)
{
    this->FreeObj("E_UI_MODEL_COLLEAGUE_GROUP-GROUPMAP", &list);
}

QList<UIVisitorInfo*> UIBaseCommonDataModel::GetVisitorInfoByUserId(QString id)
{
    QList<UIVisitorInfo*> list;
    gUIVisitorInfoManager->GetVisitorInfoByUserId(id, "UIVISITORINFO-LIST", this, &list);

    return list;
}

void UIBaseCommonDataModel::ClearVisitorInfoList(QList<UIVisitorInfo*>& list)
{
    this->FreeObj("UIVISITORINFO-LIST", &list);
}

QList<UIChatInfo*> UIBaseCommonDataModel::GetChatByUserId(QString id)
{
    QList<UIChatInfo*> chatlist;
    gUIConversationManager->GetChatByUserId(id, "UICHATINFO-LIST", this, &chatlist);
    return chatlist;
}

void UIBaseCommonDataModel::ClearChatInfoList(QList<UIChatInfo*>& list)
{
    this->FreeObj("UICHATINFO-LIST", &list);
}

QList<CommInfo*> UIBaseCommonDataModel::GetCardInfoListByKey(QString key)
{
    QList<CommInfo*> retList;
    gUIExtendConfigManager->GetCardInfoListByKey(key, "COMMINFO-LIST", this, &retList);
    return retList;
}

void UIBaseCommonDataModel::ClearCardInfoList(QList<CommInfo*>& list)
{
    this->FreeObj("COMMINFO-LIST", &list);
}

QList<UIWaitVisitorInfo*> UIBaseCommonDataModel::GetWaitQueueInfoByUserId(QString id)
{
    QList<UIWaitVisitorInfo*> waitlist;
    gUIWaitVisitorInfoManager->GetWaitQueueInfoByUserId(id, "UIWAITVISITORINFO-LIST", this, &waitlist);
    return waitlist;
}

void UIBaseCommonDataModel::ClearWaitQueueInfoList(QList<UIWaitVisitorInfo*>& list)
{
    this->FreeObj("UIWAITVISITORINFO-LIST", &list);
}

UIContactGroupInfo* UIBaseCommonDataModel::GetColleagueGroupByID(QString id)
{
    UIContactGroupInfo* group = nullptr;
    do
    {
        if (m_contactGroupInfoMapList.find(id) != m_contactGroupInfoMapList.end())
        {
            group = m_contactGroupInfoMapList[id];
        }
        if (group)
        {
            break;
        }
        group = (UIContactGroupInfo*)gUIColleagueManager->GetColleagueGroupByID(id, ConstStringHelper::m_cUICONTACTGROUPINFO, this);
        if (group)
        {
            m_contactGroupInfoMapList.insert(id, group);
        }
    } while (false);

    return group;
}

QList<UIChatInfo*> UIBaseCommonDataModel::GetCustomerNewChatListByCustomerId(QString id)
{
    QList<UIChatInfo*> tempMyInfoList;
    DbUiService::GetInstance()->ChatService()->GetCustomerNewChatListByCustomerId(id, "UICHATINFO-LIST", this, &tempMyInfoList);
    return tempMyInfoList;
}

UICommonWordInfo* UIBaseCommonDataModel::GetCommonWordInfoById(QString id)
{
    UICommonWordInfo* tempRet = nullptr;
    tempRet                   = (UICommonWordInfo*)gUICommonWordManager->GetCommonWordInfoById(id, ConstStringHelper::m_cUICOMMONWORDINFO, this);
    return tempRet;
}

UICommonWordInfo* UIBaseCommonDataModel::GetCommonWordGroupById(QString id, QString gid)
{
    UICommonWordInfo* tempRet = nullptr;
    tempRet                   = (UICommonWordInfo*)gUICommonWordManager->GetCommonWordGroupById(id, gid, ConstStringHelper::m_cUICOMMONWORDINFO, this);
    return tempRet;
}

void UIBaseCommonDataModel::ClearCommonWordInfoList(QList<UICommonWordInfo*>& list)
{
    this->FreeObj("UICOMMONWORDINFO-LIST", &list);
}

UIChatInfo* UIBaseCommonDataModel::GetChatInfoByChatId(QString id)
{
    QMutexLocker locker(&m_mutex);
    UIChatInfo*  info = nullptr;
    do
    {
        if (m_chatInfoMapList.find(id) != m_chatInfoMapList.end())
        {
            info = m_chatInfoMapList[id];
            break;
        }
        info = (UIChatInfo*)gUIConversationManager->GetChatInfoByChatId(id, ConstStringHelper::m_cUICHATINFO, this);
        if (info)
        {
            m_chatInfoMapList.insert(id, info);
        }
    } while (false);

    return info;
}

UIGroupInfo* UIBaseCommonDataModel::GetChatGroupById(QString id)
{
    UIGroupInfo* info = (UIGroupInfo*)gUIChatGroupManager->GetChatGroupById(id, ConstStringHelper::m_cUIGROUPINFO, this);
    return info;
}

QMap<QString, UIWaitVisitorInfo*> UIBaseCommonDataModel::GetWaitQueueList()
{
    QMap<QString, UIWaitVisitorInfo*> waitQueueList;
    gUIWaitVisitorInfoManager->GetWaitQueueList(ConstStringHelper::m_cUIWAITVISITORINFO_VISITORMAP, this, &waitQueueList);
    return waitQueueList;
}

void UIBaseCommonDataModel::ClearWaitQueueListMap(QMap<QString, UIWaitVisitorInfo*>& list)
{
    this->FreeObj(ConstStringHelper::m_cUIWAITVISITORINFO_VISITORMAP, &list);
}

QMap<QString, UIChatInfo*> UIBaseCommonDataModel::GetAllCacheChatList()
{
    QMap<QString, UIChatInfo*> allCacheList;
    gUIConversationManager->GetAllCacheChatList(ConstStringHelper::m_cUICHATINFO_CHATMAP, this, &allCacheList);
    return allCacheList;
}

void UIBaseCommonDataModel::ClearChatListMap(QMap<QString, UIChatInfo*>& list)
{
    this->FreeObj(ConstStringHelper::m_cUICHATINFO_CHATMAP, &list);
}

QMap<QString, UIVisitorInfo*> UIBaseCommonDataModel::GetCurrentVisitorInfoList()
{
    QMap<QString, UIVisitorInfo*> onlineMap;
    DbUiService::GetInstance()->VisitorService()->GetCurrentVisitorInfo(ConstStringHelper::m_cUIVISITORINFO_VISITORMAP, this, &onlineMap);
    return onlineMap;
}

void UIBaseCommonDataModel::ClearVisitorInfoListMap(QMap<QString, UIVisitorInfo*>& list)
{
    this->FreeObj(ConstStringHelper::m_cUIVISITORINFO_VISITORMAP, &list);
}

void UIBaseCommonDataModel::R_OtherSettingChanged()
{
    FreeObj("UIBaseCommonDataModel_MAP", &m_systemTipMapList);
}

void UIBaseCommonDataModel::R_UpdateChat(UIChatInfo info)
{
    RemoveChatInfoById(info.ChatId());
}

void UIBaseCommonDataModel::R_DelChat(QString chatId, QString vid)
{
    RemoveChatInfoById(chatId);
}

void UIBaseCommonDataModel::RemoveChatInfoById(QString chatId)
{
    QMutexLocker locker(&m_mutex);
    auto         itor = m_chatInfoMapList.find(chatId);
    if (itor != m_chatInfoMapList.end())
    {
        UIChatInfo* info = itor.value();
        if (info)
        {
            info->deleteLater();
        }

        m_chatInfoMapList.erase(itor);
    }
}

void UIBaseCommonDataModel::R_UpdateVisitor(UIVisitorInfo info)
{
    RemoveVisitorInfoById(info.Vid());
}

void UIBaseCommonDataModel::R_DelVisitor(QString uid)
{
    RemoveVisitorInfoById(uid);
}

void UIBaseCommonDataModel::RemoveVisitorInfoById(QString vid)
{
    QMutexLocker locker(&m_mutex);
    auto         itor = m_visitorInfoMapList.find(vid);
    if (itor != m_visitorInfoMapList.end())
    {
        UIVisitorInfo* info = itor.value();
        if (info)
        {
            info->deleteLater();
        }

        m_visitorInfoMapList.erase(itor);
    }
}

QList<ExtendOrderInfo*> UIBaseCommonDataModel::ExtendCardOrderColsList()
{
    QList<ExtendOrderInfo*> retList;
    gUIExtendConfigManager->ExtendCardOrderColsList("EXTENDORDERINFO-LIST", this, &retList);
    return retList;
}

void UIBaseCommonDataModel::R_MessageTipSettingChanged()
{
    FreeObj("UIBaseCommonDataModel_MAP", &m_systemTipMapList);
}

UIChannelInfo* UIBaseCommonDataModel::FindChannelById(QString id)
{
    UIChannelInfo* info = (UIChannelInfo*)gUIOtherServiceManager->findChannelById(id, ConstStringHelper::m_cUICHANNELINFO, this);
    return info;
}

UISiteInfo* UIBaseCommonDataModel::FindSiteById(QString id)
{
    UISiteInfo* info = (UISiteInfo*)gUIOtherServiceManager->findSiteById(id, ConstStringHelper::m_cUISITEINFO, this);
    return info;
}

UIDeptInfo* UIBaseCommonDataModel::FindDeptInfoById(QString id)
{
    UIDeptInfo* info = (UIDeptInfo*)gUIOtherServiceManager->findDeptById(id, ConstStringHelper::m_cUISITEINFO, this);
    return info;
}

UIGroupInfo* UIBaseCommonDataModel::GetGroupById(QString id)
{
    UIGroupInfo* info = (UIGroupInfo*)gUIOtherServiceManager->findGroupById(id, ConstStringHelper::m_cUIGROUPINFO, this);
    return info;
}

bool UIBaseCommonDataModel::IsCheckByKey(QString type, QString strKey, QObject* info)
{
    Q_ASSERT(false);
    return true;
}

void UIBaseCommonDataModel::Lazy_RemoveCache(QString type, QString strKey, QObject* info)
{
}

void UIBaseCommonDataModel::Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList)
{
}

void UIBaseCommonDataModel::ClearSummaryTagList(QMap<QString, UISummarizeInfo*>& list)
{
    this->FreeObj(ConstStringHelper::m_cUISUMMARIZEINFO_MAP, &list);
}

QMap<QString, UISummarizeInfo*> UIBaseCommonDataModel::GetSummaryTagList()
{
    QMap<QString, UISummarizeInfo*> taglist;
    gUIOtherServiceManager->GetSummaryTag(ConstStringHelper::m_cUISUMMARIZEINFO_MAP, this, &taglist);
    return taglist;
}

UISummarizeInfo* UIBaseCommonDataModel::GetSummaryTagById(QString id)
{
    UISummarizeInfo* info = (UISummarizeInfo*)gUIOtherServiceManager->findSummaryTagById(id, ConstStringHelper::m_cUISUMMARIZEINFO, this);
    return info;
}

CExtendAIRobotTipInfo* UIBaseCommonDataModel::ExtendAIRobotTipInfo()
{
    CExtendAIRobotTipInfo* info = nullptr;
    info                        = (CExtendAIRobotTipInfo*)gUIExtendConfigManager->ExtendAIRobotTipInfo(ConstStringHelper::m_cEXTENDAIROBOTTIPINFO, this);
    return info;
}

UIUserRuleInfo* UIBaseCommonDataModel::FindUserRobotInfoById(QString type, QString rid)
{
    UIUserRuleInfo* ret = nullptr;
    ret                 = (UIUserRuleInfo*)gUIManRobotService->FindUserRobotInfoById(type, rid, "UIUSERRULEINFO", this);
    return ret;
}

UIReserveRobotInfo* UIBaseCommonDataModel::GetReserveRobotInfo()
{
    UIReserveRobotInfo* ret = nullptr;
    ret                     = (UIReserveRobotInfo*)gUIManRobotService->GetReserveRobotInfo(ConstStringHelper::m_cUIRESERVEROBOTINFO, this);
    return ret;
}

QList<UIMenuDataInfo*> UIBaseCommonDataModel::GetFunMenuList()
{
    QList<UIMenuDataInfo*> retList;
    gUIPersonalConfigManager->GetFunMenuList(ConstStringHelper::m_cUIMenuDataInfo_List, this, &retList);
    return retList;
}

void UIBaseCommonDataModel::R_CreateTempChat(UIChatInfo* info)
{
    if (info)
    {
        if (info->GetVisitorInfo())
            RemoveChatInfoById(info->ChatId());
    }
}
