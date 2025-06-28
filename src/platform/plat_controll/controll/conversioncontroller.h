#ifndef PLATCONVERSIONCONTROLLER_H
#define PLATCONVERSIONCONTROLLER_H

#include "plat_controll_global.h"

//#include "datafactory/ConstructionData.h"
#include "utils/Plat_EnumDefine.h"
#include "utils/UISingleton.h"
#include "data/Im_Message.h"
#include "data/MChatItemData.h"
#include "data/MContactItemData.h"
#include "data/MMonitorChatData.h"
#include "data/MMonitorVisitorData.h"
#include "data/UIChatInfo.h"
#include "data/UIContactInfo.h"
#include <QObject>


class PLAT_CONTROLL_EXPORT ConversionController : public QObject, public UISingleton<ConversionController>
{
    Q_OBJECT
    friend class UISingleton<ConversionController>; //因为基类在GetInstance()中 new T() 时会调用到A的构造函数

private:
    ConversionController(void);
    virtual ~ConversionController(void);

public:
    void Init();
    void CreateChat(const UIChatInfo &info);
    void CreateMonitorChat(const UIChatInfo &info);
    void CreateContactChat(const UIContactInfo &info);
    void DoCloseChat(QString chatid, QString vid);
    //机器人控制消息发送接口
    void DoExecSendAiControlCmd(QString chatid, QMap<QString, QString> map);

    //发送邀请文字
    void DoSendInviteText(const QString &userid, const QString &vid, const QString &text);
    void SendComposingToVisitor(const QString &chatId);
    //发送邀请
    void DoSendInvite(const QString &userid, const QString &vid);
    //发送直接对话
    void DoSendForceInvite(const QString &userid, const QString &vid);
    //接受对话
    void DoSendAcceptActionById(QString vid);
    void DoSendAcceptAction(MContactItemData *objData);
    void DoRandomAcceptAction();

    void DoQuitChatByChatId(QString chatid, bool tempDeleteCache);
    //发送邀请评价
    void SendInviteEvaluationCmd(const QString &chatId);
    //保存会话总结
    void DoSaveSummary(const QString &type, const QString &summary, const QString &chatId);
    void DoTransChat(const QString &vid, const QString &userid, const QString &chatId, bool hasHistory, const QString &summary);
    void DoAllocationChat(const QString &vid, const QString &groupId, const QString &userid);

    void DoTransChatFormOtherToOther(const QString &vid, const QString &chatid, const QString &fromUserid, const QString &toUserId);
    void DoHiddenTransChat(const QString &vid, const QString &userid, const QString &chatId, bool hasHistory, const QString &summary, bool hidden);
    //撤消消息
    void DoRevokeMessage(const QString &chatId, QString recordId);
  //发送文件或截图。
   void UploadFile(MChatItemData *objData, QString fileName, EMESSAGETYPE msgType);
    //上传文件
    void UploadFile(const QString &fileName, const QString &chatId, EMESSAGETYPE msgType, EUSERTYPE type, bool advance = true);
    void DoShieldVisitor(const QString &id, const QString &ip, const QString &type, const QString &expires, const QString &note, const QString &chatId, int flag, const QString &scope, const QString &groupId);
    void SendQuitSameVisitorChatMessageCmd(const QString &chatid, const QString &vid);

private slots:
    void R_OnClickMonitorChatItem(MMonitorChatData *dataObj);
    void R_OnJYMainTreeMenuClick(QString type, MChatItemData *obj, EUINODEMODELTYPE nodeType);
    void R_OnClickChatItemData(MChatItemData *obj, EUINODEMODELTYPE nodeType);
    void R_OnBeginDoubleClickChatItemData(MChatItemData *obj, EUINODEMODELTYPE nodeType);
    void R_OnContactSingleClick(MContactItemData *item);
    void R_OnContactDoubleClick(MContactItemData *item);
    void R_OnCommonTreeMenuClick(QString type, MContactItemData *item);
    void R_CloseChat(MChatItemData *dataObj);
    void R_Inputting(MChatItemData *objData, QString message);
    void R_HiddenTransChat(MChatItemData *objData, QString userid);
    void R_OnChatItemCloseClicked(QString vid, QString cid);
    void R_OnInviteEvaluationBtnClick(QString chatId, QString vid);
    void R_OnSendFile(MChatItemData *dataObj, QString fileName, EMESSAGETYPE msgType);
    void R_DoShieldVisitor(const QString &id, const QString &ip, const QString &type, const QString &expires, const QString &note, const QString &chatId, int flag, const QString &scope, const QString &groupId);
    void R_OnTransferChat(const QString &vid, const QString &userid, const QString &chatId, bool hasHistory, const QString &summary);
    void R_OnClickButtonMonitorVisitorItem(QString type, MMonitorVisitorData *dataObj);
};

#endif
