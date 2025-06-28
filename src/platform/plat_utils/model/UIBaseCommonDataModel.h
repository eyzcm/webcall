#pragma once
#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>


#include "UIModelBase.h"
#include "utils/UISingleton.h"
#include "utils/UISubject.h"
#include "ui/common/CommonStruct.h"
#include "data/ExtendAIRobotTipInfo.h"
#include "data/ExtendInfo.h"
#include "data/ExtendOrderInfo.h"
#include "datafactory/UIBaseFactory.h"
#include "data/UICardInfo.h"
#include "data/UIChannelInfo.h"
#include "data/UIChatInfo.h"
#include "data/UIContactInfo.h"
#include "data/UIDeptInfo.h"
#include "data/UIExtSystemTipData.h"
#include "data/UIGroupInfo.h"
#include "data/UIHistoryInfo.h"
#include "data/UIMenuDataInfo.h"
#include "data/UIReserveRobotInfo.h"
#include "data/UISiteInfo.h"
#include "data/UISummarizeInfo.h"
#include "data/UISystemTipData.h"
#include "data/UITrackInfo.h"
#include "data/UIUserRuleInfo.h"
#include "data/UIVisitorFilterInfo.h"

class UIBaseCommonDataModel : public UIModelBase, public UISingleton<UIBaseCommonDataModel>
{
    Q_OBJECT

    friend class UISingleton<UIBaseCommonDataModel>;

private:
    UIBaseCommonDataModel();
    virtual ~UIBaseCommonDataModel();

public:
    virtual void Lazy_RemoveCache(QString type, QString strKey, QObject* info);
    virtual void Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList);
    virtual bool IsCheckByKey(QString type, QString strKey, QObject* info);

    void Init();

    /* 曹振超 */
public:
    //判断是否存在该标签
    bool FindSummaryTagByName(QString strName);

    //获取/清理 访客来源类型
    QMap<QString, EngineData*> GetEngineMap();
    void                       ClearEngineMap(QMap<QString, EngineData*>& map);

    //获取/清理 访客来源渠道
    QMap<QString, UIChannelInfo*> GetChannelMap();
    void                          ClearChannelMap(QMap<QString, UIChannelInfo*>& map);

    //获取/清理 访客子站点
    QMap<QString, UISiteInfo*> GetSubsiteMap();
    void                       ClearChannelMap(QMap<QString, UISiteInfo*>& map);

    //获取/设置 访客信息
    UIVisitorFilterInfo* GetUIVisitorFilterInfo();
    void                 SetUIVisitorFilterInfo(UIVisitorFilterInfo* info);

public:
    UIReserveRobotInfo* GetReserveRobotInfo();
    UIChannelInfo*      FindChannelById(QString id);
    UISiteInfo*         FindSiteById(QString id);
    UIDeptInfo*         FindDeptInfoById(QString id);

    UISystemTipData* GetSystemTipByKey(QString type);

    UIChatInfo* GetChatByChatId(QString id, int isNoUiData = 0);
    UIChatInfo* GetChatInfoByChatId(QString id);
    UIChatInfo* GetChatByVid(QString id);
    void        UpdateChat(UIChatInfo* info);

    UIVisitorInfo*      GetVisitorInfoById(QString id);
    UIContactGroupInfo* GetColleagueGroupByID(QString id);
    UIContactInfo*      GetColleagueMemberByID(QString id);
    UIContactInfo*      GetColleagueMemberByIDIgnoreExist(QString id);
    UIGroupInfo*        GetChatGroupById(QString id);
    UITrackInfo*        GetCurrentTrackByVid(QString id);

    UIGroupInfo* GetGroupById(QString id);

    UICommonWordInfo* GetCommonWordInfoById(QString id);
    UICommonWordInfo* GetCommonWordGroupById(QString id, QString gid);

    UICardInfo*        GetVisitorCardByUserId(QString id);
    UIWaitVisitorInfo* GetWaitQueueInfoById(QString id);
    CExtendInfo*       GetExtendInfoByKey(QString id);

    QList<CTabItemInfo*> GetTabInfoList();
    void                 ClearTabInfoList(QList<CTabItemInfo*>& list);

    QList<UIHistoryInfo*> GetHistoryListByChatId(QString chatid);
    void                  ClearHistoryList(QList<UIHistoryInfo*>& list);

    QList<UITrackInfo*> GetTrackInfoByVid(QString id);
    void                ClearTrackInfoList(QList<UITrackInfo*>& list);

    CExtendAIRobotTipInfo* ExtendAIRobotTipInfo();

    QList<ExtendOrderInfo*> ExtendOrderInfoList();
    QList<ExtendOrderInfo*> ExtendCardOrderColsList();
    void                    ClearExtendOrderInfoList(QList<ExtendOrderInfo*>& list);

    QList<CommInfo*> GetCardInfoListByKey(QString key);
    void             ClearCardInfoList(QList<CommInfo*>& list);

    QList<MContactItemData*> GetCommonwordUser();
    QList<MContactItemData*> GetColleagueMemberList();
    QList<MContactItemData*> GetColleagueMemberListByGroupID(QString id);
    void                     ClearColleagueMemberList(QList<MContactItemData*>& list);

    QMap<QString, MContactItemData*> GetColleagueGroupList();
    void                             ClearColleagueGroupList(QMap<QString, MContactItemData*>& list);

    UISummarizeInfo* GetSummaryTagById(QString id);

    QMap<QString, UISummarizeInfo*> GetSummaryTagList();
    void                            ClearSummaryTagList(QMap<QString, UISummarizeInfo*>& list);

    QList<UISystemTipData*> DefaultSystemTipList();
    QList<UISystemTipData*> CustomSystemTipList();
    void                    ClearSystemTipList(QList<UISystemTipData*>& list);

    QList<UIExtSystemTipData*> CustomExtSystemTipList();
    void                       ClearExtSystemTipList(QList<UIExtSystemTipData*>& list);

    QList<UIVisitorInfo*> GetVisitorInfoByUserId(QString id);
    void                  ClearVisitorInfoList(QList<UIVisitorInfo*>& list);
    QList<UIChatInfo*>    GetChatByUserId(QString id);
    void                  ClearChatInfoList(QList<UIChatInfo*>& list);

    QList<UIWaitVisitorInfo*> GetWaitQueueInfoByUserId(QString id);
    void                      ClearWaitQueueInfoList(QList<UIWaitVisitorInfo*>& list);

    QList<UIChatInfo*> GetCustomerNewChatListByCustomerId(QString id);

    
    void                     ClearCommonWordInfoList(QList<UICommonWordInfo*>& list);

    CommInfo* GetIconInfoByKey(const QString& key);
    CommInfo* GetTopMenuByKey(const QString& key);

    QMap<QString, UIWaitVisitorInfo*> GetWaitQueueList();
    void                              ClearWaitQueueListMap(QMap<QString, UIWaitVisitorInfo*>& list);

    QMap<QString, UIChatInfo*> GetAllCacheChatList();
    void                       ClearChatListMap(QMap<QString, UIChatInfo*>& list);

    QMap<QString, UIVisitorInfo*> GetCurrentVisitorInfoList();
    void                          ClearVisitorInfoListMap(QMap<QString, UIVisitorInfo*>& list);

    QList<UIMenuDataInfo*> GetFunMenuList();

    UIUserRuleInfo* FindUserRobotInfoById(QString type, QString rid);

public slots:
    void R_UpdateChat(UIChatInfo info);

private slots:
    void R_CreateTempChat(UIChatInfo* info);
    void R_OtherSettingChanged();
    void R_MessageTipSettingChanged();

    void R_DelChat(QString chatId, QString vid);

    void R_UpdateVisitor(UIVisitorInfo info);
    void R_DelVisitor(QString uid);

private:
    UISystemTipData* GetSystemTipCaCheByKey(QString strKey);

    void RemoveChatInfoById(QString chatId);
    void RemoveVisitorInfoById(QString vid);

private:
    QMap<QString, UISystemTipData*>    m_systemTipMapList;
    QMap<QString, UIChatInfo*>         m_chatInfoMapList;
    QMap<QString, UIVisitorInfo*>      m_visitorInfoMapList;
    QMap<QString, UIContactGroupInfo*> m_contactGroupInfoMapList;
    QMap<QString, UIContactInfo*>      m_contactInfoMapList;
    QMap<QString, UIContactInfo*>      m_contactInfoTempMapList;

    QMutex m_mutex;
};
