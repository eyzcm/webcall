#pragma once
#include <QObject>

//todo 9999 调整文件分别到平台和业务上

class ConstStringHelper
{
public:
    static const int OKCode       = 200;
    static const int NoReplyCode  = 0;
    static const int ModleVerCode = 1;

    static const int ECUnknownProtocol = 90100;

    static const int HeartAttack = 10;

    // static const int HeartPace = 15*1000;
    // static const int ParentHeartPace = 5*1000;
    // zcmtest
    static const int HeartPace       = 150 * 1000;
    static const int ParentHeartPace = 50 * 1000;

    static const QString ModuleName;
    static const QString ModuleVersion;
    static const QString ProtocolFormat;
    static const QString troy;
    static const QString stateCode;

    static const QString req;
    static const QString stat;
    static const QString reason;

    static const QString EasyLiao;
    static const QString WxPhoneOpt;
	static const QString OutdataConsole;

    static const QString RECORD_WEBPROCESS;
    static const QString CONSOLE_WEBPROCESS;
    static const QString CRM_WEBPROCESS;
    static const QString REPORT_WEBPROCESS;
    static const QString WEBPROCESS;
    static const QString CardPush_WebProcess;
	static const QString  Log_Console;

    static const QString RouteMonitor;

    static const QString LogInfo;
    static const QString BizKeepAlive;
    static const QString BizResponse;

    static const QString NodeName;
    static const QString NodeVersion;

    static const QString BizModuleInfo;
    static const QString m_cBody;
    static const QString m_cProtocolTag;

    static const QString m_cVisitorMonitorTcpManager;
    static const QString m_cTcpManager;

    static const QString m_cHttpApiUriFormat;

    static const QString m_cWaitQueue;
    static const QString m_cMySelf;
    static const QString m_cOnlineVisitor;

    static const QString m_cTalkWaitQueue;
    static const QString m_cTalkColleague;
    static const QString m_cTalkAllChatVisitor;
    static const QString m_cTalkAllMonitorVisitor;

    static const QString m_cCHATING;
    static const QString m_cINVITING;
    static const QString m_cTRANSFERING;
    static const QString m_cINNERCHATING;

    static const QString m_cModifyPassword;
    static const QString m_cLockWnd;
    static const QString m_cAutoResponse;
    static const QString m_cCommonWord;
    static const QString m_cVisitorShield;
    static const QString m_cLocalSetting;
    static const QString m_cTipSetting;

    static const QString m_cEditCard;
    static const QString m_cSummary;

    //查找访客
    static const QString m_cFindVisitor;
    //屏蔽此客人
    static const QString m_cShieldVisitor;
    //客户分类
    static const QString m_cSettingVisitor;
    //更改/查看客户资料
    static const QString m_cModifyVisitorInfo;
    //移除对话，结束此对话
    static const QString m_cKillChat;
    //发送文件
    static const QString m_cSendFile;
    //推送网页或图片
    static const QString m_cPushWebOrPicture;
    //直接对话
    static const QString m_cDirectChat;
    //接受对话
    static const QString m_cAcceptChat;
    //转接此对话
    static const QString m_cTransChat;
    //邀请对话
    static const QString m_cInviteChat;
    //访客筛选
    static const QString m_cFilterVisitor;
    //查看好友对象
    static const QString m_cViewContact;
    //内部好友对话
    static const QString m_cInnerContactChat;
    //监控对话
    static const QString m_cMonitorVisitorChat;

    static const QString m_cOnline;
    static const QString m_cBusy;
    static const QString m_cLeave;
    static const QString m_cLogout;
    static const QString m_cQuit;
    static const QString m_cOpenMainWnd;

    static const QString m_cWaiting;

    static const QString m_cNavTransChat;
    static const QString m_cNavModifyCard;
    static const QString m_cNavSettingVisitor;
    static const QString m_cNavSettingChat;
    static const QString m_cNavShieldVisitor;
    static const QString m_cNavFilterVisitor;
    static const QString m_cNavContact;
    static const QString m_cNavInnerChat;
    static const QString m_cNavLoginLog;
    static const QString m_cNavViewCard;
    static const QString m_cNavViewMessage;
    static const QString m_cNavViewRegister;
    static const QString m_cNavViewChat;
    static const QString m_cNavProcessDiagnosis;
    static const QString m_cNavReportAnalysis;
    static const QString m_cNavFreePhone;

    static const QString m_cLIST;

    static const QString m_cUIDEPTINFO_MAP;
    static const QString m_cUISUMMARIZEINFO_MAP;
    static const QString m_cUICHANNELINFO_MAP;
    static const QString m_cUISITEINFO_MAP;
    static const QString m_cUIGROUPINFO_MAP;

    static const QString m_cUICONTACTINFO_MEMBERMAP;
    static const QString m_cUICONTACTGROUPINFO_GROUPMAP;
    static const QString m_cUICHATINFO_CHATMAP;
    static const QString m_cUIVISITORINFO_VISITORMAP;
    static const QString m_cUIWAITVISITORINFO_VISITORMAP;

    static const QString m_cCloudlogin;
    static const QString m_cUIManRobotInfo;
    static const QString m_cUIMANROBOTINFO_MAP;
    static const QString m_cUIManRobotRoleInfo_MAP;

    static const QString m_cUICommonWordReplayInfo;
    static const QString m_cUICommonWordReplayInfo_List;

    static const QString m_cUIROBOTTOHUMANYSRULEINFO;
    static const QString m_cUIROBOTTOHUMANYSRULEINFO_MAP;

    static const QString m_cUIMenuDataInfo;
    static const QString m_cUIMenuDataInfo_List;

    static const QString m_cUIContactRuleDataInfo;
    static const QString m_cUIContactRuleDataInfo_List;



    static const QString m_cUIRESERVEROBOTINFO;
    static const QString m_cUIVISITORINFO;
    static const QString m_cUICONTACTINFO;
    static const QString m_cUICHATINFO;
    static const QString m_cUIWAITVISITORINFO;
    static const QString m_cUICONTACTGROUPINFO;
    static const QString m_cUIVISITORCOL;
    static const QString m_cUICOMMONWORDINFO;
    static const QString m_cCOMMINFO;
    static const QString m_cUIHISTORYINFO;
    static const QString m_cEXTENDORDERINFO;
    static const QString m_cEXTENDAIROBOTTIPINFO;
    static const QString m_cUISYSTEMTIPDATA;
    static const QString m_cUIEXTSYSTEMTIPDATA;
    static const QString m_cENGINEDATA;

    static const QString m_cCTABITEMINFO;
    static const QString m_cUITRACKINFO;
    static const QString m_cUICARDINFO;
    static const QString m_cUIGROUPINFO;
    static const QString m_cUISUMMARIZEINFO;

    static const QString m_cUISITEINFO;
    static const QString m_cUIDEPTINFO;
    static const QString m_cUICHANNELINFO;

    static const QString BizDemoProcotol;

    //个人设置-其他设置.-登录与离开
    static const QString m_cAutoStart;
    static const QString m_cAutoLogin;
    //自动变成离开状态 默认是0或者空，如果不为0，那么就标识开启，值为几分钟之后变为离开状态。
    static const QString m_cAutoLeaveState;
    static const QString m_cRememberPassword;

    //个人设置-其他设置.-对话与名片
    //对话自动清除, 默认是0或者空，如果不为0，那么就标识开启，值为几分钟之后清除。
    static const QString m_cChatAutoClear;
    //显示未说话访客
    static const QString m_cNoShowVisitor;
    //显示说话的访客
    static const QString m_cOnlyShowSpeakVisitor;


    //同时抓取
    static const QString m_cSameTime;
    //自动抓取手机号码
    static const QString m_cAutoMobile;
    //自动抓取电话号码
    static const QString m_cAutoTelephone;

    // CZC_自动抓取QQ
    static const QString m_cAutoQQ;

    // CZC_自动抓取微信
    static const QString m_cAutoWX;

    // CZC_自动抓取地区（手机号归属地）
    static const QString m_cAutoMobileArea;

    static const QString m_cAutoSaveInfo;

    static const QString m_cAutoSaveCardInfo;

    //个人设置-其他设置.-对话与名片
    //智能输入
    static const QString m_cIntelligentInput;
    //系统日志。
    static const QString m_cSystemLog;

    //新教育版 曹振超 : 个人设置--系统配置--对话与名片--提示新建名片
    static const QString m_cPromptCreateNewCard;

    // CZC_高级筛选访客
    static const QString m_cUIVisitorFilterInfo;

    // CZC_柠檬爱美_2019年11月26日
    static const QString m_cUILemonLovesBeauty;

    // CZC_对话自动清除, 默认是0或者空，如果不为0，那么就标识开启，值为几秒之后清除。
    static const QString m_cChatAutoClear_Second;

    // CZC_个人设置-常规设置 : 关闭主面板时操作
    static const QString m_cCloseMainWindow_Type;

    /* 【曹振超】【消息提醒类型】 */
    static const QString m_cNewVisit;                  //您新到一个访客
    static const QString m_cNewChat;                   //您新到一个对话
    static const QString m_cNewMsg;                    //您新到一条消息
    static const QString m_cNewTransChat;              //您新到一个转接对话
    static const QString m_cNewFriendTransChat;        //同事新到一个转接对话
    static const QString m_cCustomerNewChat;           //同事新到一个对话
    static const QString m_cNewQueueVisit;             //您新到一个排队访客
    static const QString m_cCustomerNewMsg;            //同事新到一条消息
    static const QString m_cRobotUnlearnedWords;       //机器人未学习话术提醒
    static const QString m_cSaveCardSuccess;           //您保存名片成功/访客留联成功
    static const QString m_cVisitorMsgNoReply_Self;    //访客消息未回复声音提醒（自己的对话）
    static const QString m_cVisitorMsgNoReply_Monitor; //访客消息未回复声音提醒（监控的对话）
    static const QString m_cSaveFriendCard;            //同事保存名片成功/访客留联成功

    /* 【曹振超】【系统设置】 */
    static const QString m_cOnlinePrompt;  //上线提示音
    static const QString m_cOutlinePrompt; //下线提示音

    //历史对话记录排序方式
    static const QString m_cChatHistorySort_Type;

    static const QString m_cAIRule_Type_Keyword; //关键词配置
    static const QString m_cChat_Type_Summarize; //对话总结标签
};
