#pragma once
#include <QMultiMap>
#include <QObject>

#include "UIModelBase.h"
#include "utils/UISingleton.h"
#include "data/MContactItemData.h"
#include "data/UIChatInfo.h"
#include "data/UIContactGroupInfo.h"
#include "data/UIContactInfo.h"
#include "data/UIVisitorInfo.h"
#include "data/UIWaitVisitorInfo.h"
#include "plat_model_global.h"
#include <utils/UISingleton.h>

class PLAT_MODEL_EXPORT UIContactTreeModel : public UIModelBase, public UISingleton<UIContactTreeModel>
{

    Q_OBJECT

    friend class UISingleton<UIContactTreeModel>;

private:
    UIContactTreeModel();
    virtual ~UIContactTreeModel();

public:
    void SetCurrentUserId(QString curUserId);

    virtual void Lazy_RemoveCache(QString type, QString strKey, QObject* info);
    virtual void Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList);
    virtual bool IsCheckByKey(QString type, QString strKey, QObject* info);

public:
    //初始化联系人数
    void Init();

    void InitData();

    //添加排队列表
    void AddQueueVisitorList();

    //添加同事列表
    void AddColleagueList();
    //添加监控列表
    void AddMonitorVisitorList();
    //添加对话列表
    void AddChatList();

    //添加同事部门分组
    void AddColleagueGroup(const UIContactGroupInfo& info);
    //在部门节点下，添加同事
    void AddColleagueInGroup(const UIContactInfo& info, QString departmentId);
    //临时移除网站访客
    void RemoveMonitorVisitorList(QStringList idList);

    //永久移除网站访客
    void DeleteMonitorVisitorList(QStringList idList);

    //添加单个排队访客
    void AddQueueVisitor(const UIWaitVisitorInfo& info);
    //删除单个排队访客
    void RemoveQueueVisitor(const UIWaitVisitorInfo& info);
    void RemoveQueueVisitorById(const QString& id);
    //添加单个对话
    void AddChatInfo(const UIChatInfo& info);
    //添加单个对话
    void AddMyChatInfo(const UIChatInfo& info);

    //删除对话节点
    void RemoveChatInfo(const UIChatInfo& info);

    //关闭对话
    void RemoveChatInfo(const QString& vid, const QString& chatId);

    void RemoveChatColleagueVisitor(QString vid);

    //添加单个访客
    void AddMonitorVisitorInfo(UIVisitorInfo& info);
    //临时移除监控访客，将访客移入到已离开节点
    void RemoveMonitorVisitorInfo(UIVisitorInfo& info);
    //永久删除监控访客，将访客从联系人列表删除
    void DeleteMonitorVisitorInfo(UIVisitorInfo& info);

    //客服在线状态变化
    void ColleagueStateChange(QString id, EPRESENCESTATUS oldState, EPRESENCESTATUS newState, CUSTOMER_LOGIN_TYPE loginType);
    //访客名称变化
    void VisitorNameChange(QString staticId, QString strNewName);
    //访客对话状态变化
    void VisitorChatStateChange(QString vid, QString chatid, ECHATITEMSTATE newState);

    void MonitorVisitorStateChange(QString vid, EVISITORSTATUS newState, EVISITORSTATUS oldState);

private slots:
    void R_OnCommonTreeMenuClick(QString type, MContactItemData* item);
    void R_OnContactDoubleClick(MContactItemData* item);
    void R_OnChatItemCloseClicked(QString vid, QString cid);
    void R_OnJYMainTreeMenuClick(QString type, MChatItemData* obj, EUINODEMODELTYPE nodeType);
    void R_OnDoubleClickChatItemData(MChatItemData* obj, EUINODEMODELTYPE nodeType);

    void RemoveQueueVisitor(QString vid);
    //////////////////////////////////////////////////////////////////////////
    void R_AddChat(UIChatInfo info);
    void R_UpdateChat(UIChatInfo info);
    void R_DelChat(QString chatId, QString vid);

    void R_AddVisitor(UIVisitorInfo info);
    void R_UpdateVisitor(UIVisitorInfo info);
    void R_DelVisitor(QString uid);
    //////////////////////////////////////////////////////////////////////////

    void R_CreateTempChat(UIChatInfo* pInfo);
    void R_ShowColleagueList();
    void R_GetVisitorInfoResult(QString userid, UICardInfo* info, int type);
    void R_GetWaitQueueEnd();
    void R_CustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType);
    void R_AddWaitQueueInfo(UIWaitVisitorInfo* pinfo);
    void R_RemoveWaitQueueInfoByVid(QString fromId);
    void R_SetVisitorStatus(QString vid, EVISITORSTATUS newStatus, EVISITORSTATUS oldStatus);
    void R_RemoveVMVisitorList(const QStringList& list);
    void R_RemoveTempVMVisitorList(const QStringList& list);
    void R_CreateTransferChat(UIChatInfo* pinfo, QString msgData);
    void R_AddVisitorInfo(UIVisitorInfo* pinfo);

    void R_ReceiveChatCloseMsg(QString chatid, QString vid, QString closeType);
    //////////////////////////////////////////////////////////////////////////
private:
    void UpdateMonitorVisitorOnlineVisitorState(QString vid, EVISITORSTATUS newState);
    void RemoveMonitorVisitorById(QString id);
    void DeleteMonitorVisitor(QString id);
    void AddMonitorVisitorById(QString id);

private:
    void NotifyOnlineMonitorVisitor(QString actionType, UIVisitorInfo& info);
    void NotifyOfflineMonitorVisitor(QString actionType, UIVisitorInfo& info);

    void NotifyOnlineMonitorVisitor(QString actionType, MContactItemData& info);
    void NotifyOfflineMonitorVisitor(QString actionType, MContactItemData& info);

    void NotifyRemoveOnlineMonitorVisitorById(QString id, QString actionType);
    void NotifyDeleteOnlineMonitorVisitorById(QString id, QString actionType);

    void NotifyDeleteOfflineMonitorVisitorById(QString id, QString actionType);
    void NotifyRemoveOfflineMonitorVisitorById(QString id, QString actionType);

    void NotifyAddOnlineMonitorVisitor(MContactItemData& info, QString actionType);
    void NotifyAddOfflineMonitorVisitor(MContactItemData& info, QString actionType);

private:
    void AddQueueRootNote();
    void AddMonitorChatRootNote();
    void AddColleagueRootNote();
    void AddMonitorVisitorRootNote();
    void AddRootNode(QString actionType, EUINODEMODELTYPE eType);
    void RemoveChatVisitorByIdList(QStringList idList);

    void AddColleagueNodeAllChat(QString id);
    void MergerColleagueChatNode(QString id);
    void RemoveNoChatColleagueNode();

private:
    void UpdateMonitorChatColleague(const QString& customerId, bool isAdd = true);

public:
    bool              IsFindContactItemNodeById(const QString& id, EUINODEMODELTYPE eType);
    MContactItemData* GetContactItemNodeById(const QString& id, EUINODEMODELTYPE eType);
    MContactItemData* CreateContactItemData(QString strUUid, QString vid, QString userid, QString chatid, QString groupid, QString departmentId, EUINODEMODELTYPE etype, QString strName);

    QList<MContactItemData*> GetContactItemNodeByCustomerId(const QString& id);
    void                     RemoveModelCacheById(QString id, EUINODEMODELTYPE etype);

private:
    QString                               m_currentUserId;
    QList<MContactItemData*>              m_modelContactItemDataList;
    QMultiMap<QString, MContactItemData*> m_modelContactItemDataIndexList;

    QMultiMap<QString, MContactItemData*> m_modelCustomerContactItemDataIndexList;
};
