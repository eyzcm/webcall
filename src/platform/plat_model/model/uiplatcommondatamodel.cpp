#include "uiplatcommondatamodel.h"
#include "utils/ConstStringHelper.h"
#include "serviceinterface/dbuiplatservice.h"
#include "service/PlatCommonSignalService.h"

UIPlatCommonDataModel::UIPlatCommonDataModel()
{
    m_mainWndSettingInfo = nullptr;
}

void UIPlatCommonDataModel::Init()
{
    connect(PlatCommonSignalService::GetInstance(), SIGNAL(S_UpdateChat(UIChatInfo)), this, SLOT(R_UpdateChat(UIChatInfo)));
    connect(PlatCommonSignalService::GetInstance(), SIGNAL(S_DelChat(QString, QString, QMap<QString, QString>)), this, SLOT(R_DelChat(QString, QString, QMap<QString, QString>)));

    connect(PlatCommonSignalService::GetInstance(), SIGNAL(S_UpdateVisitor(UIVisitorInfo)), this, SLOT(R_UpdateVisitor(UIVisitorInfo)));
    connect(PlatCommonSignalService::GetInstance(), SIGNAL(S_DelVisitor(QString)), this, SLOT(R_DelVisitor(QString)));
    connect(PlatCommonSignalService::GetInstance(), SIGNAL(S_CreateTempChat(UIChatInfo*)), this, SLOT(R_CreateTempChat(UIChatInfo*)));
}


void UIPlatCommonDataModel::UpdateVisitorUpdateData(UIVisitorUploadDataInfo* info)
{
    if (!info->ChatId().isEmpty())
    {
        UIChatInfo* data = this->GetChatInfoByChatId(info->ChatId());
        if (data)
        {
            info->UserId(data->CustomerID());
            if (data->GetVisitorInfo())
            {
                info->VisitorStaticId(data->GetVisitorInfo()->StaticId());
                UIChatUserInfo* chatUserInfo = data->GetVisitorInfo();

                if (chatUserInfo && chatUserInfo->OtherInfos())
                {
                    QString resolution = chatUserInfo->OtherInfos()->GetValueByKey("resolution");
                    if (!resolution.isEmpty())
                    {
                        info->VisitorResolutionRatio(resolution);
                    }

                    QString chroma = chatUserInfo->OtherInfos()->GetValueByKey("chroma");
                    if (!chroma.isEmpty())
                    {
                        info->VisitorColorDepth(chroma);
                    }

                    QString os = chatUserInfo->OtherInfos()->GetValueByKey("os");
                    if (!os.isEmpty())
                    {
                        info->VisitorOs(os);
                    }

                    QString device = chatUserInfo->OtherInfos()->GetValueByKey("deviceModel");
                    if (!device.isEmpty())
                    {
                        info->UserDevice(device);
                    }

                    QString lang = chatUserInfo->OtherInfos()->GetValueByKey("lang");
                    if (!lang.isEmpty())
                    {
                        info->VisitorLang(lang);
                    }
                    QString userAgent = chatUserInfo->OtherInfos()->GetValueByKey("userAgent");
                    if (!userAgent.isEmpty())
                    {
                        info->VisitorUserAgent(userAgent);
                    }

                    QString ie = chatUserInfo->OtherInfos()->GetValueByKey("ie");
                    if (!ie.isEmpty())
                    {
                        info->VisitorBrowser(ie);
                    }
                }
            }
        }
    }
    else if (!info->VisitorStaticId().isEmpty())
    {
        QList<UIVisitorInfo*> vinfolist = GetVisitorInfoByUserId(info->VisitorStaticId());
        if (vinfolist.size() > 0)
        {
            UIVisitorInfo* vinfo = vinfolist.at(0);
            info->VisitorOs(vinfo->Os());
            info->SearchEngineId(vinfo->SearchEngineId().toInt());
            info->VisitorBrowser(vinfo->Ie());
            info->VisitorColorDepth(vinfo->Chroma());
            info->VisitorResolutionRatio(vinfo->Resolution());
            info->VisitorDeviceModel(vinfo->DeviceModel());
            info->VisitorDeviceType(vinfo->Device());
            info->VisitorLang(vinfo->Lang());
            info->VisitorUserAgent(vinfo->UserAgent());
        }
    }
}


UIChatInfo* UIPlatCommonDataModel::GetChatByChatId(QString id, int isNoUiData)
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
        info = (UIChatInfo*) gUIChatServiceManager->GetChatByChatId(id, ConstStringHelper::m_cUICHATINFO, this);
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
                m_chatInfoMapList.remove(id);
            }
            m_chatInfoMapList.insert(id, info);
        }
        else
        {
            info = (UIChatInfo*) gUIChatRecordServiceManager->findChatRecordById(id, ConstStringHelper::m_cUICHATINFO, this);
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
                    m_chatInfoMapList.remove(id);
                }
                m_chatInfoMapList.insert(id, info);
            }
        }

    } while (false);

    return info;
}

void UIPlatCommonDataModel::UpdateChat(UIChatInfo* info)
{
    if (info)
    {
        if (m_chatInfoMapList.contains(info->ChatId()))
        {
            m_chatInfoMapList[info->ChatId()]->AiChatFlag(info->AiChatFlag());
        }
    }
}


UIChatInfo* UIPlatCommonDataModel::GetChatByVid(QString id)
{
    QMutexLocker locker(&m_mutex);
    UIChatInfo*  info = nullptr;
    do
    {
        info = (UIChatInfo*) gUIConversationManager->GetChatByVid(id, ConstStringHelper::m_cUICHATINFO, this);
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
                m_chatInfoMapList.remove(info->ChatId());
            }
            m_chatInfoMapList.insert(info->ChatId(), info);
        }
    } while (false);

    return info;
}

UIContactInfo* UIPlatCommonDataModel::GetColleagueMemberByID(QString id)
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
        info = (UIContactInfo*) gUIColleagueManager->GetColleagueMemberByID(id, ConstStringHelper::m_cUICONTACTINFO, this);
        if (info)
        {
            m_contactInfoMapList.insert(id, info);
        }
    } while (false);
    return info;
}


QList<UIVisitorInfo*> UIPlatCommonDataModel::GetVisitorInfoByUserId(QString id)
{
    QList<UIVisitorInfo*> list;
    gUIVisitorInfoManager->GetVisitorInfoByUserId(id, "UIVISITORINFO-LIST", this, &list);

    return list;
}

void UIPlatCommonDataModel::ClearVisitorInfoList(QList<UIVisitorInfo*>& list)
{
    this->FreeObj("UIVISITORINFO-LIST", &list);
}

QList<UIChatInfo*> UIPlatCommonDataModel::GetChatByUserId(QString id)
{
    QList<UIChatInfo*> chatlist;
    gUIConversationManager->GetChatByUserId(id, "UICHATINFO-LIST", this, &chatlist);
    return chatlist;
}

void UIPlatCommonDataModel::ClearChatInfoList(QList<UIChatInfo*>& list)
{
    this->FreeObj("UICHATINFO-LIST", &list);
}


QList<UIWaitVisitorInfo*> UIPlatCommonDataModel::GetWaitQueueInfoByUserId(QString id)
{
    QList<UIWaitVisitorInfo*> waitlist;
    gUIWaitVisitorInfoManager->GetWaitQueueInfoByUserId(id, "UIWAITVISITORINFO-LIST", this, &waitlist);
    return waitlist;
}

void UIPlatCommonDataModel::ClearWaitQueueInfoList(QList<UIWaitVisitorInfo*>& list)
{
    this->FreeObj("UIWAITVISITORINFO-LIST", &list);
}


bool UIPlatCommonDataModel::IsCheckByKey(QString type, QString strKey, QObject* info)
{
    Q_ASSERT(false);
    return true;
}

void UIPlatCommonDataModel::Lazy_RemoveCache(QString type, QString strKey, QObject* info)
{
}

void UIPlatCommonDataModel::Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList)
{
}


QString UIPlatCommonDataModel::GetSystemConfigByKey(const QString& key, const QString& defaultString)
{

    return gUISystemConfigManager->GetSystemConfigByKey(key, defaultString);
}


UIChatInfo* UIPlatCommonDataModel::GetChatInfoByChatId(QString id)
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
        info = (UIChatInfo*) gUIConversationManager->GetChatInfoByChatId(id, ConstStringHelper::m_cUICHATINFO, this);
        if (info)
        {
            m_chatInfoMapList.insert(id, info);
        }
    } while (false);

    return info;
}


QList<UIHistoryInfo*> UIPlatCommonDataModel::GetHistoryListByChatId(QString chatid)
{
    QList<UIHistoryInfo*> retList;
    gUIHistoryManager->GetHistoryListByChatId(chatid, "UIHISTORYINFO-LIST", this, &retList);
    return retList;
}


void UIPlatCommonDataModel::ClearHistoryList(QList<UIHistoryInfo*>& list)
{
    this->FreeObj("UIHISTORYINFO-LIST", &list);
}

UIVisitorInfo* UIPlatCommonDataModel::GetVisitorInfoById(QString id)
{
    UIVisitorInfo* visitor = nullptr;
    do
    {
        if (m_visitorInfoMapList.find(id) != m_visitorInfoMapList.end())
        {
            visitor = m_visitorInfoMapList[id];
            break;
        }
        visitor = (UIVisitorInfo*) gUIVisitorInfoManager->GetVisitorInfoById(id, ConstStringHelper::m_cUIVISITORINFO, this);
        if (visitor)
        {
            m_visitorInfoMapList.insert(id, visitor);
        }

    } while (false);

    return visitor;
}


void UIPlatCommonDataModel::MainWndSettingInfo( UIMainWndSettingInfo  * info)
{
    m_mainWndSettingInfo = info;
}


UIMainWndSettingInfo * UIPlatCommonDataModel::MainWndSettingInfo( )
{
    return m_mainWndSettingInfo;
}


UISystemTipData* UIPlatCommonDataModel::GetSystemTipByKey(QString type)
{
    UISystemTipData* ret = nullptr ;//GetSystemTipCaCheByKey(type);
    do
    {
        if (ret)
        {
            break;
        }


        UISystemTipData* defaultTip = (UISystemTipData*) gUISystemConfigManager->GetSystemTipByKey(type, ConstStringHelper::m_cUISYSTEMTIPDATA, this);
        if (defaultTip)
        {
            //m_systemTipMapList.insert(type, defaultTip);
            ret = defaultTip;
            break;
        }

    } while (false);
    return ret;
}

MChatItemData * UIPlatCommonDataModel::CurrentChatItem()
{
    //todo 9999
    return nullptr;
}

CommInfo* UIPlatCommonDataModel::GetIconInfoByKey(const QString& key)
{
    return new CommInfo();
}


void UIPlatCommonDataModel::RemoveVisitorInfoById(QString vid)
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


void UIPlatCommonDataModel::RemoveChatInfoById(QString chatId)
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

UIContactInfo* UIPlatCommonDataModel::GetColleagueMemberByIDIgnoreExist(QString id)
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
        info = (UIContactInfo*) gUIColleagueManager->GetColleagueMemberByID(id, ConstStringHelper::m_cUICONTACTINFO, this);
        if (info == nullptr)
        {
            //好友列表
            info = (UIContactInfo*) gUIColleagueManager->GetFriendByID(id, ConstStringHelper::m_cUICONTACTINFO, this);
        }

        if (info)
        {
            m_contactInfoMapList.insert(id, info);
        }
        else
        {
            if ((!id.isEmpty()) && (id.length() < 30))
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

UIContactGroupInfo* UIPlatCommonDataModel::GetColleagueGroupByID(QString id)
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
        group = (UIContactGroupInfo*) gUIColleagueManager->GetColleagueGroupByID(id, ConstStringHelper::m_cUICONTACTGROUPINFO, this);
        if (group)
        {
            m_contactGroupInfoMapList.insert(id, group);
        }
    } while (false);

    return group;
}
QList<UIChatInfo*> UIPlatCommonDataModel::GetCustomerNewChatListByCustomerId(QString id)
{
    QList<UIChatInfo*> tempMyInfoList;
    gUIChatServiceManager->GetCustomerNewChatListByCustomerId(id, "UICHATINFO-LIST", this, &tempMyInfoList);
    return tempMyInfoList;
}


void UIPlatCommonDataModel::R_UpdateChat(UIChatInfo info)
{
    RemoveChatInfoById(info.ChatId());
}

void UIPlatCommonDataModel::R_DelChat(QString chatId, QString vid, QMap<QString, QString> extendInfo)
{
    RemoveChatInfoById(chatId);
}


void UIPlatCommonDataModel::R_UpdateVisitor(UIVisitorInfo info)
{
    RemoveVisitorInfoById(info.Vid());
}

void UIPlatCommonDataModel::R_DelVisitor(QString uid)
{
    RemoveVisitorInfoById(uid);
}

void UIPlatCommonDataModel::R_CreateTempChat(UIChatInfo* info)
{
    if (info)
    {
        if (info->GetVisitorInfo())
            RemoveChatInfoById(info->ChatId());
    }
}



