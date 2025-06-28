#ifndef UIPLATCOMMONDATAMODEL_H
#define UIPLATCOMMONDATAMODEL_H

#include <QObject>
#include "UIModelBase.h"
#include <utils/UISingleton.h>
#include "data/UIVisitorUploadDataInfo.h"
#include <data/UIHistoryInfo.h>
#include "data/uimainwndsettinginfo.h"
#include "data/UISystemTipData.h"
#include "plat_model_global.h"


class PLAT_MODEL_EXPORT UIPlatCommonDataModel : public UIModelBase, public UISingleton<UIPlatCommonDataModel>
{
    Q_OBJECT
    friend class UISingleton<UIPlatCommonDataModel>;
public:

    void Init();
    static UIPlatCommonDataModel * Instance()
    {
        return UIPlatCommonDataModel::GetInstance();
    }

    UIPlatCommonDataModel();
    virtual void Lazy_RemoveCache(QString type, QString strKey, QObject* info);

    virtual void Lazy_RemoveCache(QString type, QString strKey, QList<QObject*> infoList);

    virtual bool IsCheckByKey(QString type, QString strKey, QObject* info);


    UIChatInfo* GetChatByChatId(QString id, int isNoUiData = 0);
    UIChatInfo* GetChatInfoByChatId(QString id);
    UIChatInfo* GetChatByVid(QString id);
    void        UpdateChat(UIChatInfo* info);
    void UpdateVisitorUpdateData(UIVisitorUploadDataInfo* info);

    QList<UIVisitorInfo*> GetVisitorInfoByUserId(QString id);
    void                  ClearVisitorInfoList(QList<UIVisitorInfo*>& list);
    QList<UIChatInfo*>    GetChatByUserId(QString id);
    void                  ClearChatInfoList(QList<UIChatInfo*>& list);

    QList<UIWaitVisitorInfo*> GetWaitQueueInfoByUserId(QString id);
    void                      ClearWaitQueueInfoList(QList<UIWaitVisitorInfo*>& list);

    QList<UIChatInfo*> GetCustomerNewChatListByCustomerId(QString id);
    UIContactInfo* GetColleagueMemberByID(QString id);
    QString GetSystemConfigByKey(const QString& key, const QString& defaultString);
    QList<UIHistoryInfo*> GetHistoryListByChatId(QString chatid);
    void ClearHistoryList(QList<UIHistoryInfo*>& list);
    UIVisitorInfo* GetVisitorInfoById(QString id);

    //todo 9999
    CommInfo* GetIconInfoByKey(const QString& key);

    void MainWndSettingInfo( UIMainWndSettingInfo  * info);
    UIMainWndSettingInfo * MainWndSettingInfo( );
    UISystemTipData* GetSystemTipByKey(QString type);
    MChatItemData * CurrentChatItem();
    UIContactInfo*      GetColleagueMemberByIDIgnoreExist(QString id);
    UIContactGroupInfo* GetColleagueGroupByID(QString id);

private:
    void RemoveChatInfoById(QString chatId);
    void RemoveVisitorInfoById(QString vid);

signals:

public slots:
    void R_UpdateChat(UIChatInfo info);
    void R_DelChat(QString chatId, QString vid, QMap<QString, QString> extendInfo);

    void R_UpdateVisitor(UIVisitorInfo info);
    void R_DelVisitor(QString uid);

    void R_CreateTempChat(UIChatInfo* info);


private:
    QMap<QString, UIChatInfo*>         m_chatInfoMapList;
    QMap<QString, UIVisitorInfo*>      m_visitorInfoMapList;
    QMap<QString, UIContactGroupInfo*> m_contactGroupInfoMapList;
    QMap<QString, UIContactInfo*>      m_contactInfoMapList;
    QMap<QString, UIContactInfo*>      m_contactInfoTempMapList;

   UIMainWndSettingInfo  * m_mainWndSettingInfo;

};

#endif // UIPLATCOMMONDATAMODEL_H
