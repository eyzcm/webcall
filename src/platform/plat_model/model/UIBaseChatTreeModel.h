#pragma once
#include "utils/Plat_EnumDefine.h"
#include "UIModelBase.h"
#include "utils/UISingleton.h"
#include "data/CommInfo.h"
#include "data/Im_Message.h"
#include "data/MChatItemData.h"
#include "data/MContactItemData.h"
#include "data/UIChatInfo.h"
#include "data/UIContactInfo.h"
#include "data/UIVisitorInfo.h"
#include "data/UIWaitVisitorInfo.h"
#include <QListWidgetItem>
#include <QObject>

#include "plat_model_global.h"

class PLAT_MODEL_EXPORT UIBaseChatTreeModel : public UIModelBase
{
    Q_OBJECT

public:
    UIBaseChatTreeModel(void);
    virtual ~UIBaseChatTreeModel(void);

public:
    virtual void Lazy_RemoveCache(QString type, QString strKey, QObject* info);
    virtual void Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList);

    virtual bool IsCheckByKey(QString type, QString strKey, QObject* info);

signals:
    void S_RemoveChatItem(QString vid);

public:
    void           Init();
    void           SetCurrentUserId(QString curUserId);
    MChatItemData* GetCurrentChatItemData() const;
    void           SetCurrentChatItemData(MChatItemData* val);
    void           NotifyRobotTakeOver(MChatItemData* objData);

protected:
    virtual void DoRemoveChatItemData(MChatItemData* dataObj);
    virtual void DoChatItemCloseClicked(QString vid, QString chatId);
    virtual void DoRobotCloseChat(MChatItemData* dataObj);
    virtual void DoAddChatInfo(UIChatInfo info);
    virtual void DoUpdateChatInfo(UIChatInfo info);
    virtual void DoDeleteChatInfo(QString chatId, QString vid);
    virtual void DoCreateTempChat(UIChatInfo* pInfo);
    virtual void DoShowNotice(CommInfo* pInfo);
    virtual void DoGetVisitorInfoResult(QString userid, UICardInfo* cardInfo, int type);
    virtual void DoReceiveMessage(const Im_Message& msg);
    virtual void DoCustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType);
    virtual void DoOtherSettingChanged();
    virtual void DoCreateTransferChat(UIChatInfo* pinfo, QString msgData);
    virtual void DoReceiveChatCloseMsg(QString chatid, QString vid, QString closeType);

    virtual void AddCacheToList(MChatItemData* itemObj);

private:
    void RemoveCache(const QString& id);
    void RemoveCache(const MChatItemData* pdata);

public:


    bool HasRecordHistoryByChatId(const QString& chatid, const QString& vid);

    /************************************************************************/
    /* QString type :invite, chat, closechat,monitor
    /* 通过对话添加对话节点，
    /* UIChatInfo Obj: 对话的底层对象
    /************************************************************************/
    void NotifyAddChatItem(const UIChatInfo& obj, QString type, bool isSelect = false);

    /************************************************************************/
    /* 添加同事节点对话
    /* UIContactInfo obj: 同事对象
    /************************************************************************/
    void NotifyAddChatItem(const UIContactInfo& obj, bool isSelect = false);

    /************************************************************************/
    /* 添加访客信息浏览节点
    /* UIVisitorInfo obj 访客信息
    /************************************************************************/
    void NotifyAddChatItem(const UIVisitorInfo& obj, bool isSelect = false);

    /************************************************************************/
    /* 添加排队访客信息查询节点
    /* UIWaitVisitorInfo obj 排队访客
    /************************************************************************/
    void NotifyAddChatItem(const UIWaitVisitorInfo& obj, bool isSelect = false);

    /************************************************************************/
    /* 添加通知信息节点
    /* CommInfo obj 通知信息
    /************************************************************************/
    void NotifyAddChatItem(const CommInfo& obj, bool isSelect = false);

    void AddCache(MChatItemData* item);

    bool IsFindChatItemNodeById(const QString& id);
    bool IsFindChatItemNodeByChatId(const QString& id);

    MChatItemData*        GetMChatItemDataById(const QString& id);
    MChatItemData*        GetMChatItemDataByChatId(const QString& id);
    QList<MChatItemData*> GetMChatItemDataListByStaticId(const QString& staticId);

    void NotifyTwinkleChatItem(QString chatid);

    void NotifyReceiveMessage(QString chatid);

    //清除所有结束对话
    void DoClearAllCloseChatItem();

    bool IsCheckRemoveCloseChat(MChatItemData* itemObj, ESUMMARIZEONCLOSE summarizeType);

    void NotifyRobotAutoReplayError(MChatItemData* objData);

    void NotifyChatStateChanged(QList<MChatItemData*> removeChatItemList);

    void DoCloseChat(QString vid, QString chatId);

    void CloseChat(MChatItemData* objData);

    void OtherSettingChanged();

protected:
    void NotifyCreateChatItem(MChatItemData* item);
    void NotifyRomoveChatItem(QString vid);
    void NotifyModifyChatItem(MChatItemData* item);
    void NotifyModifyChatItem(QString type, MChatItemData* item);
    void NotifySelectChatItem(MChatItemData* item);
    void NotifyTwinkleChatItem(MChatItemData* item);

public:
    void RemoveChatItemById(const QString& id);
    void RemoveChatItem(MChatItemData* item);
    void ColleagueStateChange(QString id, EPRESENCESTATUS oldState, EPRESENCESTATUS newState, CUSTOMER_LOGIN_TYPE loginType);
    void ColleagueStateChanged(EPRESENCESTATUS oldState, EPRESENCESTATUS newState, CUSTOMER_LOGIN_TYPE loginType, MChatItemData* obj);

    void VisitorNameChange(QString staticId, QString strNewName);
    void VisitorChatStateChange(QString vid, QString chatid, ECHATITEMSTATE newState);

    void NotifyChatStateChanged(MChatItemData* obj, ECHATITEMSTATE newState);

private slots:
    void R_GetVisitorInfoResult(QString userid, UICardInfo* info, int type);
    void R_OnChatItemSelectChanged(MChatItemData* oldObj, MChatItemData* currObj);
    void R_OnRemoveChatItemData(MChatItemData* obj);
    void R_OnChatItemCloseClickedEnd(QString vid, QString cid);

    void R_CloseChat(MChatItemData* dataObj);
    //////////////////////////////////////////////////////////////////////////
    void R_AddChat(UIChatInfo info);
    void R_UpdateChat(UIChatInfo info);
    void R_DelChat(QString chatID, QString vid);
    //////////////////////////////////////////////////////////////////////////

    void R_CreateTempChat(UIChatInfo* pInfo);
    void R_ShowNotice(CommInfo* pInfo);

    void R_ReceiveMessage(const Im_Message& msg);
    void R_CustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType);
   void R_OtherSettingChanged();
    void R_CreateTransferChat(UIChatInfo* pinfo, QString msgData);

    void R_ReceiveChatCloseMsg(QString chatid, QString vid, QString closeType);
    //////////////////////////////////////////////////////////////////////////

private:
    MChatItemData* CreateMChatItemData(const UIChatInfo& obj, QString type);
    ECHATITEMSTATE GetNodeStateByChatInfo(const UIChatInfo& obj, QString type);

    MChatItemData* CreateMChatItemData(const CommInfo& obj);

private:
    void RemoveChatVisitorByIdList(QStringList idList);

protected:
    QString                       m_currentUserId;
    QList<MChatItemData*>         m_modelList;
    QMap<QString, MChatItemData*> m_modelIndexList;
    QStringList                   m_chatIdList;
    MChatItemData*                m_currChatItemData;
};
