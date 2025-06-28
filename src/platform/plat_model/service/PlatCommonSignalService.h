#pragma once

#include <QEvent>
#include <QObject>
#include <QStringList>

#include "include/json/value.h"
#include "utils/Plat_EnumDefine.h"
#include "utils/UISingleton.h"
#include "data/CommInfo.h"

#include "data/Im_Message.h"
#include "data/MChatItemData.h"

#include "data/MContactItemData.h"
#include "data/MFontData.h"
#include "data/MMonitorChatData.h"
#include "data/MMonitorVisitorData.h"
#include "data/UIChatInfo.h"
#include "data/UIContactInfo.h"
#include "data/UIVisitorInfo.h"
#include "data/UIWaitVisitorInfo.h"
#include "data/MUICardInfoData.h"

#include "plat_model_global.h"

class PLAT_MODEL_EXPORT PlatCommonSignalService : public QObject //, public UISingleton<PlatCommonSignalService>
{
    Q_OBJECT
   // friend class UISingleton<PlatCommonSignalService>; //因为基类在


private:

    class Garbo
    {
    public:
        ~Garbo();
    };
    static PlatCommonSignalService *m_instance;

private:
    PlatCommonSignalService(void);
    virtual ~PlatCommonSignalService(void);

public:
    static PlatCommonSignalService *GetInstance();
    void InitSignal();
    static PlatCommonSignalService * Instance()
    {
        return GetInstance();
    }

signals:

    void S_UpdateThirdCardTab(MUICardInfoData *data);


    void S_TakeoverStatus(QString chatId, CHAT_TAKEOVER_FLAG flag);
       void S_aiImgchange(QString Chatid, QString img);
       void S_AddSystemInfo(QString vid, QString chatId, QString msg);
       void S_JoininChat(AI_CHAT_STATUS type, QString vid, QString chatId, QString customerId, int flag, int statusChange);
       void S_GetViewEnv();
       void S_UploadOtherFileEnd(QString fileType, QString jsonstr);
    void S_SaveHistoryMessage(QString body, QString chatid, QString from, EMESSAGETYPE type);
    void S_SendRobotStopByChatID(QString chatId );
    void S_GetCurrentChatRecord(QString chatID);
    void S_UpdateNoReadNum(MChatItemData *objData, int nums);
    void S_ReSendFile(QString filename);
    void S_NotifyRobotNoReply(QString chatId, QString recordId);
    void S_SetAiPanelScale();
    void S_PopPersonWebWindow(QString key, QString jsonval, QString url);
    void S_OpenAIWindow(QString key, QString jsonval);
    void S_ReturnAIData(QString key, QString jsonval);
    void S_ReturnPersonSetData(QString key, QString jsonval);
    void S_ShowAiTab(int type, QString val);
    void S_GetCompanyGlobalConfResult(QString code, QString reason, QString body);
    void S_InitTopMenu();
    void S_ShowIeWndByUri(EURITYPE type, int openType, QString subURiParam);
    //置顶窗口
    void S_SetMainWndTop(bool isTopWnd);

    //双击中央对话区图片，打开图片浏览器
    void S_OpenPicture(QString url);
    //最大化图片浏览器窗口
    void S_MaxParentWindow();
    //设置图片浏览器默认宽度
    void S_SetParentWindowSize(QRect rect);
    //访客消息未回复声音提醒
    void S_VisitorMsgNoReplySelf_Sound();
    void S_VisitorMsgNoReplyMonitor_Sound();
    //获取绑定的微信信息
    void S_GetWeixinBindInfoResult(QString code, QString reson, QString body);
    //解除绑定
    void S_CancleWeixinBindResult(QString code, QString reson, QString body);
    //获取绑定的手机号信息
    void S_GetPhoneBindInfoResult(QString code, QString reson, QString body);
    //解除手机绑定
    void S_CanclePhoneBindResult(QString code, QString reson, QString body);
    //账号授权登陆
    void S_AdminAuthLogin();
    //右下角弹窗打开Crm窗口
    void S_OpenCrm(QString url);
    //发送表单消息
    void S_SendTable_FormatData(QString val);
    //接收到表单消息
    void S_ReceiveTable_FormatData(QString vid);
    //发送表单，但未勾选任何项
    void S_SendTable_PromptNoCheck();
    // CZC：对话唤醒
    void S_ChatAwaken_ContactTree(QString searching, QString awake, QString awakeInfo);
    void S_ChatAwaken_ChatList(QString searching, QString awake, QString awakeInfo);
    // CZC：辅助功能区--展示易聊助手面板
    void S_ShowNotice_Clicked();
    // CZC：主树--展示易聊助手网页
    void S_BtnNotice_Clicked();
    // CZC：显示通知公告、新手指引气泡提示
    void S_BtnCloseNotice_Clicked();
    void S_UpdataeRecordId(QString chatId, QString callbackId, QString recordId);
    // CZC:输入功能区返回上下键，切换对话item的条目
    void S_KeyUp();
    void S_KeyDown();
    // CZC:显隐常用语名片面板
    void S_ShowCommonWordAndCardPanel(bool show);
    // CZC:显示自定义悬浮窗口
    void S_ShowCustomHoverPanel(QString userID);
    /* 【高级筛选访客】 */
    void S_Finished_AddressLibrary();
    //【Toast提示框-主界面】
    void S_ShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);
    //【Toast提示框-AI设置中心】
    void S_ShowToastMsg_AISettingCenter(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);
    //【Toast提示框-个人设置】
    void S_ShowToastMsg(QString msg, EYLUI_ToastType type);
    //【个人设置】【主界面】【显示设置】
    void S_CommonWordPosChanged(); //常用语展示位置改变
    void S_CommonWord_TipMessage(QString msg);
    void S_SetBtnDeleteState(bool show);
    void S_SendCommonfile(QString msg); //发送常用语文件
    void S_ClickCountChanged();

    void S_SelectEmo(QString str); //添加表情到中央输入区
    /* 曹振超：右上角功能区 */
    void S_TopRight_LockScreen_Clicked();
    void S_TopRight_ResetScale_Clicked();
    void S_ReadOneNotice();
    /* 曹振超 2019年9月23日 设置各个面板的初始尺寸 */
    void S_SetPanelScale();
    // CZC_个人设置保存成功_2019年11月26日
    void S_PersonMainSettingSaveSuccess();
    // CZC_柠檬爱美
    void S_OnRedEnvelopesClick();
    // CZC:新教育版 我的对话 item 被点击，通知联系列表
    void S_MyChatItemClicked();
    // CZC:个人设置-快捷键：切换常用语、名片窗口
    void S_Key_CommonWordSearch();
    void S_Key_OpenCard();
    void S_Key_ChatChange();
    void S_Key_CloseSelectedChat();
    // CZC:个人设置-通知客户端左上角修改用户昵称
    void S_UpdateUserNickName(QString nickName);
    // CZC:个人设置自定义提示音
    void S_DeleteCustomItem(QString key, QString name, QString fileName, QString prefabricate);
    void S_StartChat(QString chatID);
    //全局快捷键
    void S_ActivateShortcut(const quint32 keyCode, const quint32 modifiers);
    void S_AutoJoinTagBySendMsg(QString chatId);

    void S_OnClickSearchCommonWordItem(QString id);
    void S_OnDoubleClickSearchCommonWordItem(QString id);
    void S_OnSendMessage(MChatItemData *item, QString type, QString message);
    void S_OnClickSearchItem(QString vid, QString type);

    void S_OnRecentChatItemSelectChanged(MChatItemData *oldObj, MChatItemData *currObj);
    //未读消息数
    void S_OnChatItemUnreadNumber(MChatItemData *currObj, int unReadNumber);
    void S_RemoveChatItem(QString vid);
    //用户信息区域点击信号
    void S_OnLockBtnClick();
    void S_OnHelpBtnClick();
    void S_OnCustomerHelpBtnClick();
    //联系人树信号
    void S_OnContactSingleClick(MContactItemData *item);
    void S_OnContactDoubleClick(MContactItemData *item);
    void S_OnCommonTreeMenuClick(QString type, MContactItemData *item);
    //重构过后的信号：联系人节点的单击、双击
    void S_OnClickChatItemData(MChatItemData *obj, EUINODEMODELTYPE parentNodeType);
    void S_OnDoubleClickChatItemData(MChatItemData *obj, EUINODEMODELTYPE parentNodeType);
    void S_OnBeginDoubleClickChatItemData(MChatItemData *obj, EUINODEMODELTYPE parentNodeType);
    void S_OnJYMainTreeMenuClick(QString type, MChatItemData *obj, EUINODEMODELTYPE nodeType);
    void S_OnFlashingTreeView(bool isFlashing);
    //对话列表信号
    void S_MainSelectChat(QString vid);
    void S_OnAddChatItemData(MChatItemData *obj);
    void S_OnChatItemCloseClicked(QString vid, QString cid);
    void S_OnChatItemCloseClickedEnd(QString vid, QString cid);
    void S_OnMyChatItemClose_ClearComposing(QString vid);
    void S_OnRemoveChatItemData(MChatItemData *obj);
    void S_OnSelectedChatItemData(MChatItemData *obj);

    void S_ChatNumberChanged(QString val);


    //对话记录控件信号
    void S_OnSaveChatCard(MChatItemData *dataObj);
    void S_OnSendComposingToVisitor(MChatItemData *dataObj);
    void S_OnSetVisitorAutoReplyError(MChatItemData *dataObj);
    void S_OnPreSendMsg(MChatItemData *dataObj, QString content);
    void S_OnCheckSendMessageReply(MChatItemData *dataObj);
    //机器人模块信号
    void S_DoSearchFriendProtocol(MChatItemData *objData, QString mobile);
    void S_DoAddFriendProtocol(MChatItemData *objData, QString mobile);
    void S_HiddenTransChat(MChatItemData *objData, QString userid);
    void S_Inputting(MChatItemData *objData, QString message);
    void S_SaveChatCard(MChatItemData *objData);
    void S_SetChatCard(MChatItemData *objData, QString content);
    void S_CloseChat(MChatItemData *objData);
    void S_SendAutoRobotReplyMessage(MChatItemData *objData, QString message, bool isJoin, QString actionId, CommInfo otherInfo);
    void S_SetAutoJoinTag(QString vid, QString chatId);
    void S_SystemGetCard(MChatItemData *objData, QString mobile, QString tel, QString weixin, QString qq, QString type);
    void S_SaveCard_MobileArea(MChatItemData *objData, QString area, QString type);
    //个人常用语界面信号
    void S_SaveCommonWordToLocal();

    //字体变化
    void S_OnFontChanged(MFontData *obj);
    //工具栏信号
    void S_SummaryResult(QString vid, QString chatId, bool isOk);
    void S_OnInviteEvaluationBtnClick(QString chatId, QString vid);
    void S_OnSetMainWndVisible(bool isShow);
    void S_OnCopyAllByVid(QString vid);
    void S_OnFontBtnClicked(bool isDown);
    void S_OnTransferChat(const QString &vid, const QString &userid, const QString &chatId, bool hasHistory, const QString &summary);
    void S_OnSendClpboardImage(MChatItemData *chatItem);
    void S_OnSendFile(EMESSAGETYPE messageType, QString fileName, EMESSAGETYPE msgType);
    void S_DoShieldVisitor(const QString &id, const QString &ip, const QString &type, const QString &expires, const QString &note, const QString &chatId, int flag, const QString &scope, const QString &groupid);

    void S_OnHistoryBtnClicked(MChatItemData *dataObj, bool isDwon);
    void S_OnInsertWeiXin(QString chatId, QString groupId);
    void S_SendTable(QString chatId, QString groupId);
    void S_RemoveFormat();
    void S_ScreenShotFinish();
    //访客监测列表信号
    void S_OnClickMonitorVisitorItem(MMonitorVisitorData *dataObj);
    void S_OnClickButtonMonitorVisitorItem(QString type, MMonitorVisitorData *dataObj);
    //访客监控列表信号
    void S_OnClickMonitorChatItem(MMonitorChatData *dataObj);
    void S_OnClick_UIYLMonitorVisitorItem(MChatItemData *dataObj);
    //个人头像信号
    void S_OnShowPersonalSetting();
    void S_DoQuit();
    void S_DoSetState(EPRESENCESTATUS status);
    void S_DoStateChange(EPRESENCESTATUS fromstate,EPRESENCESTATUS tostate);

    void S_SaveVisitorCard(bool isNew);
    //底层信号
    void S_SendMessageToAi(const Im_Message &msg);
    void S_SendReceiptToAi(const QMap<QString, QString> &map);
    void S_GetUserSessionhistoryListResult(QString code, QString reason, QString body, QString customerId, QString chatlist);
    void S_ueUploadFileEnd(QString fileType, EUSERTYPE type, QString fileName, QString msgTo, EMESSAGETYPE msgType);
    void S_SendFileMsg(QString fileType, EUSERTYPE type, QString fileName, QString msgTo, EMESSAGETYPE msgType);
    void S_SendFilefinishMsg(QString fileType, EUSERTYPE type, QString fileName, QString msgTo, EMESSAGETYPE msgType); //上传文件完成,通知html
    void S_CreateTempChat(UIChatInfo *pInfo);
    void S_CreateChatoffline(UIChatInfo *pInfo);
    void S_Record_CreateTempChat(UIChatInfo *pInfo);
    void S_ReceiveChatCloseMsg(QString chatid, QString vid, QString closeType);
    void S_CreateTransferChat(UIChatInfo *pinfo, QString msgData);
    void S_ShowColleagueList();
    //常用语获取完成
    void S_GetCommonWordColleageListResult();
    void S_SetCommonWordinfo(QString id);
    void S_AddTagFinished(QString id, QString uuid);
    void S_GetVisitorHistoryChat(QString vid, QString body);
    void S_GetCurrentChatRecordFinished(QString chatid, QString body);
    void S_GetVisitorHistoryChatByDouyinOffline(QString UserId, QString body);
    void S_ShowNotice(CommInfo *pInfo);
    void S_GetVisitorColsEnd();
    void S_GetVisitorInfoResult(QString userid, UICardInfo * info, int type);
    void S_GetWaitQueueEnd();
    void S_CustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE logintype);
    void S_AddWaitQueueInfo(UIWaitVisitorInfo *pinfo);
    void S_RemoveWaitQueueInfoByVid(QString fromId);
    void S_AddVisitorInfo(UIVisitorInfo *pinfo);
    void S_SetVisitorStatus(QString vid, EVISITORSTATUS newStatus, EVISITORSTATUS oldStatus);
    void S_RemoveVMVisitorList(const QStringList &list);
    void S_RemoveTempVMVisitorList(const QStringList &list);
    void S_ReceiveMessage(const Im_Message &msg);
    void S_ReceiveAIMessage(QString vid, QString chatId, QString body);
    void S_Received_Im_AI_Robot_Message(int type, QString chatId, QString customerId, int delaytime);
    void S_OtherSettingChanged();
    void S_MessageTipSettingChanged();
    void S_ChatSettingChanged();
    void S_GetCompanyInfoEnd();
    void S_GetCompanyInfoStatisticsEnd();

    void S_GetChatFriendlyEnd();
    void S_AddChat(UIChatInfo info);
    void S_UpdateChat(UIChatInfo info);
    void S_DelChat(QString chatId, QString customerID, QMap<QString, QString> extendInfo);
    void S_AddVisitor(UIVisitorInfo info);
    void S_UpdateVisitor(UIVisitorInfo info);

    void S_AddFriend(UIContactInfo info);
    void S_UpdateFriend(UIContactInfo info);
    void S_DelFriend(QString uid);
    void S_VisitorFilterChanged();
    void S_BackupColorChanged();
    void S_ShowSettingChanged();
    void S_CurrentSkinChanged();
    void S_OpertorSettingChanged();
    void S_RobotAutoReplyError(MChatItemData *objData, const QString &content, QString code);
    void S_RobotNoReplyError(QString chatId, QString recordId);
    void S_ShowRebotList(QList<CommInfo *> &list);
    void S_AddEditText(QString text);
    void S_AddReferredMsg(QString jsonText);

    //搜索规则列表点击节点
    void S_OnClickManRobotRule(QString id);
    void S_OnClickManRobotRuleSearchItem(QString id);
    // CZC：人机结合-关键词规则列表项被点击
    void S_ManRule_KeyWordItem_Clicked(QString id);
    void S_ManRule_KeyWordSearchItem_Clicked(QString id);
    void S_StopTwinkleTip();
    void S_DoubleClickTrayIcon();
    void S_ManRobotTipUpdateConfig();
    void S_DoGetCompanyInfosByCompanyIdFinished(QString code, QString reason, QString body);
    void S_DoGetSummaryCategoryFinished(QString code, QString reason, QString body);
    void S_FindUserAiRulesResult(QString code, QString reason, QString body);
    void S_SetAiRuleSortResult(QString code, QString reason, QString body);
    void S_GetRobotRuleRoleConfigListResult(QString code, QString reason, QString body);
    void S_GetRobotRuleConfigListResult(QString code, QString reason, QString body);
    void S_GetUserRuleListResult(QString code, QString reason, QString body);
    void S_GetUnreadAnnouncementResult(QString code, QString reason, QString body, int count);
    void S_DelManRobotRule(QString id);

    void S_CloseChatAutoSaveCard(QString userId, QString vid);
    void S_UserDelManRobotRule(QString type, QString id);

    void S_GetRobotToHumanysListResult(QString code, QString reason, QString body);
    void S_AddOrUpdateReserveRobotRelationResult(QString code, QString reason, QString body);
    void S_GetReserveRobotsListResult(QString code, QString reason, QString body);
    void S_GetReserveRobotRelationListResult(QString code, QString reason, QString body);
    void S_ReserveRobotInfo();
    void S_UpdateReserverRobotInfo();
    void S_GetQuickReplyCommonWordResult(QString code, QString reason, QString body);
    void S_SaveQuickReplyCommonWordResult(QString code, QString reason, QString body, QString jsonData, int flag);
    void S_DelQuickReplyCommonWordResult(QString code, QString reason, QString body, QString ids);
    void S_Update_package_download_progress(qint64 bytesSent, qint64 bytesTotal); //更新包下载进度
    void S_Update_package_download_completed();                                   //更新包下载完成
    void S_ShowAnnouncementNotice(QString title, QString content, QString time);  //显示公告通知
    void S_ShowAnnouncementDetails();                                             //显示公告详情
    void S_SetUnreadMsgNum(int c, QString title, QString content, QString time);  //设置未读消息数

    void S_GetMenuInfoListResult();                                               //获取顶部功能菜单完成
    void S_Request_Get_dialog_list();                                             //请求获取可以转移对话的列表
    void S_Get_dialog_list(Json::Value v);                                        //获取可以转移对话的列表
    void S_OnChatSummaryBtnBtnClick();                                            //总结标签
    void S_OnEchoSummary(QString chatId);                                         //回显总结标签
    void S_ExpirationReminder(QString ExpirationReminder);                        //到期提醒

    void S_DelVisitor(QString uid);


public:
    void DoUpdateThirdCardTab(MUICardInfoData *data);
    void DoOpenPicture(QString url);
    void DoExpirationReminder(QString ExpirationReminder); //SXL 到期提醒
    void DODoQuit();
      void DoTakeoverStatus(QString chatId, CHAT_TAKEOVER_FLAG flag);
      void DoAddSystemInfo(QString vid, QString chatId, QString msg);
      void DoRecord_CreateTempChat(UIChatInfo *pInfo);

      void DoSendMessageToAi(const Im_Message &msg);
      void DoSendReceiptToAi(const QMap<QString, QString> &map);
      void DoGetViewEnv();
    void DoSaveHistoryMessage(QString body, QString chatid, QString from, EMESSAGETYPE type);
    void DoSendRobotStopByChatID(QString chatID);
    void DoGetCurrentChatRecord(QString chatID);
    void DoShowAnnouncementNotice(QString title, QString content, QString time); //显示公告通知
    void DoUpdateNoReadNum(MChatItemData *objData, int nums);
    void DoAddReferredMsg(QString jsontext);
    void DoMainSelectChat(QString vid);
    void DoShowToastMsg(QString msg, EYLUI_ToastType type);
    void DoShowToastMsg(QString msg, int showTime, EYLUI_ToastShowPos showPos, EYLUI_ToastType type, QPoint tarPos);
    void DoNotifyRobotNoReply(QString chatId, QString recordId);
    void DoSetAiPanelScale();
    void DoRobotNoReplyError(QString chatId, QString recordId);
    void DoOpenAIWindow(QString key, QString jsonval);
    void DoPopPersonWebWindow(QString key, QString jsonval, QString url);
    void DoReturnAIData(QString key, QString jsonval);
    void DoReturnPersonSetData(QString key, QString jsonval);
    void DoShowAiTab(int type, QString val);
    void DoInitTopMenu();
    void DoShowIeWndByUri(EURITYPE type, int openType, QString subURiParam);
    void DoClickCountChanged();

    void DoSendClpboardImage(MChatItemData *chatItem);

    void DoSummaryResult(QString vid, QString chatId, bool isOk);
    void DoShowRebotList(QList<CommInfo *> &list);
    void DoGetRobotAnswer(QString answer);
    void DoAddEditText(QString text);
    void DoCurrentSkinChanged();
    void DoAutoJoinTagBySendMsg(QString chatId);
    void DoAddChatItemData(MChatItemData *obj);
    void DoChatItemCloseClicked(QString vid, QString cid);
    void DoSelectedChatItemData(MChatItemData *obj);
    void DoSendMessage(MChatItemData *obj, QString type, QString messageBody);

    void DoAddTagFinished(QString tagId, QString uuid);
    void DoUploadOtherFileEnd(QString fileType, QString jsonstr);
    void DoUploadFileEnd(QString fileType, EUSERTYPE type, QString fileName, QString msgTo, EMESSAGETYPE msgType);
    void DoUploadingFile(QString fileType, EUSERTYPE type, QString code, QString fileName, EMESSAGETYPE msgType);
    void DoCreateTempChat(UIChatInfo *pInfo, bool isoffline = false);
    void DoReceiveChatCloseMsg(QString chatid, QString vid, QString closeType);
    void DoCreateTransferChat(UIChatInfo *pinfo, QString msgData);
    void DoGetChatFriendlyEnd();
    void DoShowColleagueList();
    void DoGetCommonWordColleageListResult();
    void DoSetCommonWordinfo(QString id);
    void DoGetCurrentChatRecordFinished(QString chatid, QString body);
    void DoGetVisitorHistoryChat(QString vid, QString body);
    void DoGetVisitorHistoryChatByDouyinOffline(QString UserId, QString body);
    void DoShowNotice(CommInfo *pInfo);
    void DoGetVisitorColsEnd();
    void DoGetVisitorInfoResult(QString userid, UICardInfo *info, int type);
    void DoGetWaitQueueEnd();
    void DoGetCompanyInfoEnd();
    void DoGetCompanyInfoStatisticsEnd();
    void DoCustomerPresenceChanged(QString fromId, EPRESENCESTATUS newStatus, EPRESENCESTATUS oldStatus, CUSTOMER_LOGIN_TYPE loginType);
    void DoAddWaitQueueInfo(UIWaitVisitorInfo *pinfo);
    void DoRemoveWaitQueueInfoByVid(QString fromId);
    void DoAddVisitorInfo(UIVisitorInfo *pinfo);
    void DoSetVisitorStatus(QString vid, EVISITORSTATUS newStatus, EVISITORSTATUS oldStatus);
    void DoRemoveVMVisitorList(const QStringList &list);
    void DoRemoveTempVMVisitorList(const QStringList &list);
    void DoReceiveMessage(const Im_Message &msg);
    void DoReceiveAIMessage(QString vid, QString chatId, QString body);
    void DoReceived_Im_AI_Robot_Message(int type, QString chatId, QString customerId, int delaytime);
    void DoOtherSettingChanged();
    void DoMessageTipSettingChanged();
    void DoChatSettingChanged();

    void DoRobotAutoReplyError(MChatItemData *objData, const QString &content, QString code);
    void DoAddChat(UIChatInfo info);
    void DoUpdateChat(UIChatInfo info);
    void DoDelChat(QString chatId, QString customerID, QMap<QString, QString> extendInfo = QMap<QString, QString>());
    void DoAddVisitor(UIVisitorInfo info);
    void DoUpdateVisitor(UIVisitorInfo info);

    void DoAddFriend(UIContactInfo info);
    void DoUpdateFriend(UIContactInfo info);
    void DoDelFriend(QString uid);
    void DoSetState(EPRESENCESTATUS status);
    void DoStateChange(EPRESENCESTATUS fromstate,EPRESENCESTATUS tostate);
    void DoVisitorFilterChanged();
    void DoBackupColorChanged();
    void DoShowSettingChanged();
    void DoOpertorSettingChanged();
    void DoRemoveChatItem(QString vid);

    void DoGetCompanyInfosByCompanyIdFinished(QString code, QString reason, QString body);
    void DoGetSummaryCategoryFinished(QString code, QString reason, QString body);
    void DoFindUserAiRulesResult(QString code, QString reason, QString body);
    void DoSetAiRuleSortResult(QString code, QString reason, QString body);
    void DoGetRobotRuleConfigListResult(QString code, QString reason, QString body);
    void DoGetRobotRuleRoleConfigListResult(QString code, QString reason, QString body);
    void DoGetUserRuleListResult(QString code, QString reason, QString body);
    void DoGetUnreadAnnouncementResult(QString code, QString reason, QString body, int count);

    void DoCloseChatAutoSaveCard(QString userId, QString vid);
    void DoReserveRobotInfo();
    void DoSaveVisitorCard(bool isNew);
    void DoUpdate_package_download_completed();

private:

};
