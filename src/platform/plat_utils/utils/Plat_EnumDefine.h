#pragma once

#define ToastPromptTime_Sec 2

\
enum E_YL_TREE_TAB
{
    E_YL_MAIN_TREE,
    E_YL_CUSTOMER_TREE,
    E_YL_RECENT_TREE,
};


enum E_TABTYPE
{
    E_TAB_ALL_PAGE              = 0,
    E_TAB_MY_CHAT_PAGE          = 1,
    E_TAB_RECENT_CHAT_PAGE      = 2,
    E_TAB_MONITOR_CHAT_PAGE     = 3,
    E_TAB_MONITOR_VISITOR_PAGE  = 4,
    E_TAB_MONITOR_CUSTOMER_PAGE = 5,
};


enum EMESSAGESUBTYPE
{
    E_SUBMSGTYPE_MAIN                = 0,
    E_SUBMSGTYPE_REFERRED            = 1, //引用，用于message
    E_SUBMSGTYPE_SHAKE               = 2, //抖动 用于 sysem_message
    E_SUBMSGTYPE_GLOBAL              = 3,
    E_SUBMSGTYPE_WORDTOIMAGE         = 4,
    E_SUBMSGTYPE_GUIDE               = 5,
    E_SUBMSGTYPE_AUTOREPLY_CONNECT   = 6,
    E_SUBMSGTYPE_AUTOREPLY_FREQUENT  = 7,
    E_SUBMSGTYPE_AUTOREPLY_BUSY      = 8,
    E_SUBMSGTYPE_AUTOREPLY_MANUALOFF = 9,
    E_SUBMSGTYPE_AUTOREPLY_KEYWORD   = 10,
    E_SUBMSGTYPE_AUTOREPLY_HIGH      = 11,
    E_SUBMSGTYPE_BAIDU_SYSTEM        = 12
};

enum EVENTSUBTYPE
{
    E_SUBEVENT_ACKCHAT  = 1, //对话保活
    E_SUBEVENT_JOINOVER = 3  //接管对话
};





enum EUINODETYPE
{
    E_UI_NONE                = 0x00000000,
    E_UI_NODE_VISITOR        = 0x00000001, //访客
    E_UI_NODE_CHAT           = 0x00000002, //对话
    E_UI_NODE_QUEUE          = 0x00000004, //排队
    E_UI_NODE_COLLEAGUE      = 0x00000010, //客服
    E_UI_NODE_CHATGROUP      = 0x00000020, //群
    E_UI_NODE_COLLEAGUEGROUP = 0x00000040, //同事分组

    E_UI_NODE_MONITOR_ONLINE_VISITOR = 0X00000080, //访客
    E_UI_NODE_MONITOR_LEAVE_VISITOR  = 0X00000100, //访客
    E_UI_NODEE_COLLEAGUE_CHAT        = 0X00000200  //对话的客服。
};



enum ECLIENTTYPE
{
    E_CLIENT_NONE             = 0,
    E_CLIENT_MEDICAL          = 1,
    E_CLIENT_TALK99           = 2,
    E_CLIENT_ENTERPRISE       = 3,
    E_CLIENT_MERCHANTSEDITION = 4,
    E_CLIENT_EDUEDITION       = 5,
    E_CLIENT_NEWMEDICAL       = 6, //新医疗版
	E_CLIENT_WEB = 7, //web版
};

enum ELOGINSTATUS
{
    E_LOGIN_NONE             = 0,
    E_LOGIN_PRE              = 1,
    E_LOGIN_DOING            = 2,
    E_LOGIN_OK_MAINTCP       = 3, //主tcp登录成功
    E_LOGIN_ERROR            = 4,
    E_LOGIN_ERROR_MAINTCP    = 5,
    E_LOGIN_ERROR_MONITORTCP = 6,
    E_LOGIN_PRE_MONITORTCP   = 7, // monitor 链接成功
    E_LOGIN_RETRY_MAINTCP    = 8,
    E_LOGIN_RETRY_MONITORTCP = 9,
    E_LOGIN_ALLOK            = 10,
    E_LOGIN_LOGOUT           = 11,
};

//在线状态
enum EPRESENCESTATUS
{
    E_NONE    = 0,
    E_ONLINE  = 1,
    E_BUSY    = 2,
    E_LEAVE   = 3,
    E_OFFLINE = 4,
    E_WAITING = 5,
};


enum EMESSAGETYPE
{
    E_MSGTYPE_DEFAULT            = 0,
    E_MSGTYPE_EVENT_CLOSE        = 1,
    E_MSGTYPE_EVENT_END          = 2,
    E_MSGTYPE_AUTOACCEPTINVITE   = 3,
    E_MSGTYPE_COMPOSING          = 4,
    E_MSGTYPE_DENY               = 5,
    E_MSGTYPE_FILE               = 6,
    E_MSGTYPE_MESSAGE            = 7,
    E_MSGTYPE_MONITORSUCCESS     = 8,
    E_MSGTYPE_OPINION            = 9,
    E_MSGTYPE_SCREENSHOTS        = 10,
    E_MSGTYPE_TRANS              = 11,
    E_MSGTYPE_LEAVE_MSG          = 12,
    E_MSGTYPE_FREE_PHONE         = 13,
    E_EVENT_POP_MESSAGE          = 14,
    E_MSGTYPE_AUTOMONTOR         = 15,
    E_MSGTYPE_LEMON_REDPACKET    = 16,
    E_MSGTYPE_LEMON_PRODUCT      = 17,
    E_MSGTYPE_SYSMESSAGE         = 18,
    E_EXENDINFO_CHANGED          = 19,
    E_MSGTYPE_REVOKE             = 20,
    E_MSGTYPE_TOKEOVER           = 21,
    E_MSGTYPE_ACCEPTTAKEOVER     = 22,
    E_MSGTYPE_SYSMESSAGE_WELCOME = 23,
    E_MSGTYPE_READRECEIPT        = 24,
    E_MSGTYPE_ACK_RESPONSE       = 25,
    E_MSGTYPE_EVENTCUSTOM        = 26, //定制消息类型
    E_MSGTYPE_WXVOICE            = 27, //微信语音消息
    E_MSGTYPE_VIDEO              = 28, //视频文件
    E_MSGTYPE_VOICE              = 29, //音频文件
    E_MSGTYPE_IMAGE              = 30, //图片
    E_SMGTYPE_ROBOTSTART         = 31,
    E_SMGTYPE_ROBOTSTOP          = 32
};



enum EGROUPEVENTTYPE
{
    E_GROUPEVENTTYPE_DEFAULT      = 0,
    E_GROUPEVENTTYPE_ADD          = 1,
    E_GROUPEVENTTYPE_DELETE       = 2,
    E_GROUPEVENTTYPE_RENAME       = 3,
    E_GROUPEVENTTYPE_ADDMEMBER    = 4,
    E_GROUPEVENTTYPE_DELETEMEMBER = 5,
    E_GROUPEVENTTYPE_VISITOR_IN   = 6,
    E_GROUPEVENTTYPE_VISITOR_OUT  = 7,
};

enum EUSERTYPE
{
    E_USERTYPE_DEFAULT = 0,
    //客服
    E_CUSTOMER = 1,
    //访客
    E_VISITOR = 2,
    //通知
    E_NOTICE = 3,
    //监控对话
    E_MONITOR = 4,
    //群聊
    E_CHATGROUP = 5,
    E_UEDITOR   = 6,
    E_DOUYIN   = 7

};



enum EVISITORSTATUS
{
    E_VSTATUS_DEFAULT      = 0,
    E_VSTATUS_BROWSE_WEB   = 1,
    E_VSTATUS_CONVERSATION = 2,
    E_VSTATUS_INVITE       = 3,
    E_VSTATUS_LEAVE        = 4,
    E_VSTATUS_QUEUE        = 5,
    E_VSTATUS_INNER_CHAT   = 6,
    E_VSTATUS_COLLEAGUE    = 7,
};

enum EURITYPE
{
    E_Uri_ReportAnalysis   = 1,
    E_Uri_ProcessDianosis  = 2,
    E_Uri_ViewConversation = 3,
    E_Uri_ViewRegister     = 4,
    E_Uri_ViewMessage      = 5,
    E_Uri_ViewCard         = 6,
    E_Uri_VisitorFilter    = 7,
    E_Uri_AutoResponse     = 8,
    E_Uri_CommonWord       = 9,
    E_Uri_CenterManager    = 10,
    E_Uri_WeiPage          = 11,
    E_Uri_CRM              = 12,
    E_Uri_FreePhone        = 13,
    E_Uri_CardAssign       = 14,
    E_Uri_WorkOrder        = 15,
    E_Uri_EachbotAI        = 16,
    E_Uri_VerifyCard       = 17,
    E_Uri_AISettingCenter  = 18,
    E_Uri_ClueQuery        = 19,
    E_Uri_CardPush         = 20,
    E_Uri_AuthOpen         = 21,
    E_Uri_BuidWeb          = 22,
};




//新医疗版 : Toast提示窗口显示位置(父窗口中固定位置、目标窗口的附近)
enum EYLUI_ToastShowPos
{
    Enum_YL_Toast_Parent_LeftTop,    //父窗口 : 左上
    Enum_YL_Toast_Parent_LeftCenter, //父窗口 : 左中
    Enum_YL_Toast_Parent_LeftBottom, //父窗口 : 左下

    Enum_YL_Toast_Parent_CenterTop,    //父窗口 : 中上
    Enum_YL_Toast_Parent_CenterCenter, //父窗口 : 中中
    Enum_YL_Toast_Parent_CenterBottom, //父窗口 : 中下

    Enum_YL_Toast_Parent_RightTop,    //父窗口 : 右上
    Enum_YL_Toast_Parent_RightCenter, //父窗口 : 右中
    Enum_YL_Toast_Parent_RightBottom, //父窗口 : 右下

    Enum_YL_Toast_Target_LeftTop_Up,   //目标位置左上角 : 上方
    Enum_YL_Toast_Target_LeftTop_Left, //目标位置左上角 : 左侧

    Enum_YL_Toast_Target_LeftBottom_Bottom, //目标位置左下角 : 下方
    Enum_YL_Toast_Target_LeftBottom_Left,   //目标位置左下角 : 左侧

    Enum_YL_Toast_Target_RightTop_Up,    //目标位置右上角 : 上方
    Enum_YL_Toast_Target_RightTop_Right, //目标位置右上角 : 右侧

    Enum_YL_Toast_Target_RightBottom_Bottom, //目标位置右下角 : 下方
    Enum_YL_Toast_Target_RightBottom_Right,  //目标位置右下角 : 右侧

    Enum_YL_Toast_Target_Top_HCenter,    //目标位置上方 : 水平居中（目标点X轴 与 Toast窗口中点X轴一致）
    Enum_YL_Toast_Target_Bottom_HCenter, //目标位置下方 : 水平居中（目标点X轴 与 Toast窗口中点X轴一致）

    Enum_YL_Toast_Target_Left_VCenter, //目标位置左侧 : 垂直居中（目标点Y轴 与 Toast窗口中点Y轴一致）
    Enum_YL_Toast_Target_Right_VCenter //目标位置下方 : 垂直居中（目标点Y轴 与 Toast窗口中点Y轴一致）
};

//新医疗版 : Toast提示框类型
enum EYLUI_ToastType
{
    Enum_YL_Toast_Type_Error,   //错误
    Enum_YL_Toast_Type_Urgent,  //紧急
    Enum_YL_Toast_Type_Warning, //警告
    Enum_YL_Toast_Type_Common   //常规
};


enum EUSERROLE
{
    E_USER = 0,
    E_ADMIN,
    E_DEPARDADMIN
};




enum CUSTOMER_LOGIN_TYPE
{
    CUSTOMER_LOGIN_CLIENT = 0, //客户端
    CUSTOMER_LOGIN_APP    = 1, //移动端
    CUSTOMER_LOGIN_TRUST  = 2  //云端
};


enum ECHATITEMSTATE
{
    E_CHATITEM_DEFAULT = 99,
    //更新
    E_CHATITEM_SETUP = 0,
    //浏览访客信息
    E_CHATITEM_BROWSE = 1,
    //对话
    E_CHATITEM_CHAT = 2,
    //邀请
    E_CHATITEM_INVITE = 3,
    //离开
    E_CHATITEM_LEAVE = 4,
    //对话关闭
    E_CHATITEM_CHAT_CLOSE = 5,
    //监控对话
    E_CHATITEM_MONITOR_CHAT = 6,
    //监控对话关闭
    E_CHATITEM_MONITOR_CHAT_CLOSE = 7,
    //排队
    E_CHATITEM_QUEUE = 8,
};


enum EUINODEMODELTYPE
{
    E_UI_MODEL_NONE          = 0,
    E_UI_MODEL_QUEUE         = 1, //排队根节点
    E_UI_MODEL_QUEUE_VISITOR = 2, //排队根节点-访客节点

    E_UI_MODEL_MONITOR_CHAT                   = 3, //监控对话根节点
    E_UI_MODEL_MONITOR_CHAT_COLLEAGUE         = 4, //监控对话-同事节点
    E_UI_MODEL_MONITOR_CHAT_COLLEAGUE_VISITOR = 5, //监控对话-同事节点-访客节点

    E_UI_MODEL_COLLEAGUE                = 6, //客服列表根节点
    E_UI_MODEL_COLLEAGUE_GROUP          = 7, //客服列表根节点-部门分组节点
    E_UI_MODEL_COLLEAGUE_GROUP_CUSTOMER = 8, //客服列表根节点-部门分组-客服节点

    E_UI_MODEL_MONITOR_VISITOR                 = 9,  //网站浏览根节点
    E_UI_MODEL_MONITOR_VISITOR_ONLINE          = 10, //网站浏览根节点-在线访客
    E_UI_MODEL_MONITOR_VISITOR_OFFLINE         = 11, //网站浏览根节点-离线访客
    E_UI_MODEL_MONITOR_VISITOR_ONLINE_VISITOR  = 12, //网站浏览根节点-在线访客—访客节点
    E_UI_MODEL_MONITOR_VISITOR_OFFLINE_VISITOR = 13  //网站浏览根节点-离线访客—访客节点

};

//强制对话总结
enum ESUMMARIZEONCLOSE
{
    E_NO_FORCE = 0,
    E_FORCE    = 1,
    E_TIP      = 2,
};



enum AI_CHAT_STATUS
{
    CHAT_STATUS_NONE     = 0,
    CHAT_STATUS_ROBOT    = 1, //纯人工模式
    CHAT_STATUS_JOININ   = 2, //人工已介入
    CHAT_STATUS_JOINOVER = 3  //人工接管
};

enum CHAT_TAKEOVER_FLAG
{
    CHAT_TAKEOVER_NONE = 0,
    CHAT_TAKEOVER_YUNAITOMAN,
    CHAT_TAKEOVER_AITOMAN,
    CHAT_TAKEOVER_AIRULETOMAN,
    CHAT_TAKEOVER_WHIRL,
    CHAT_TAKEOVER_TRANS,
    CHAT_TAKEOVER_MUCHGUIDE,
    CHAT_TAKEOVER_FAILED //转移失败
};


enum ECHATACTION
{
    E_Action_QUIT            = 1,
    E_Action_TRANS           = 2,
    E_Action_OPINION         = 3,
    E_ACTION_GETFOCUS        = 4,
    E_ACTION_QUITSAMEVISITOR = 5,
    E_Action_SYS_QUIT        = 6,
};


enum EPERMISSIONS
{
    E_PERM_STOPCHAT = 0,
    E_PERM_MSGWITHDRAW
};

