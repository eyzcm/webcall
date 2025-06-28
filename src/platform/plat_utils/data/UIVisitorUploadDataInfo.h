#pragma once
#include "include/json/reader.h"
#include "include/json/value.h"
#include "QMap"
#include "data/CommInfo.h"

#include <QObject>
#include <QStringList>

class  UIVisitorUploadDataInfo : public QObject
{
    Q_OBJECT

public:
    UIVisitorUploadDataInfo(void);
    UIVisitorUploadDataInfo(UIVisitorUploadDataInfo* info);
    ~UIVisitorUploadDataInfo(void);

public:
    QString VisitorUploadDataId() const
    {
        return m_VisitorUploadDataId;
    }
    void VisitorUploadDataId(QString val)
    {
        m_VisitorUploadDataId = val;
    }

    QString CreateTime() const;
    void    CreateTime(QString val);

    QString FirstType() const;
    void    FirstType(QString val);

    QString SecType() const;
    void    SecType(QString val);

    QString ThirdType() const;
    void    ThirdType(QString val);

    QString CompanyId() const;
    void    CompanyId(QString val);

    QString VisitorStaticId() const;
    void    VisitorStaticId(QString val);

    QString ChatId() const;
    void    ChatId(QString val);

    int  Operator() const;
    void Operator(int val);

    QString Exts() const;
    void    Exts(QString val);

    QString ShowMethod() const;
    void    ShowMethod(QString val);

    int  SearchEngineId() const;
    void SearchEngineId(int val);

    QString VisitorBrowser() const;
    void    VisitorBrowser(QString val);

    QString VisitorDeviceType() const;
    void    VisitorDeviceType(QString val);

    QString VisitorResolutionRatio() const;
    void    VisitorResolutionRatio(QString val);

    QString VisitorOs() const;
    void    VisitorOs(QString val);

    QString VisitorDeviceModel() const;
    void    VisitorDeviceModel(QString val);

    QString VisitorUserAgent() const;
    void    VisitorUserAgent(QString val);

    QString VisitorLang() const;
    void    VisitorLang(QString val);

    QString VisitorColorDepth() const;
    void    VisitorColorDepth(QString val);

    QString UserId() const;
    void    UserId(QString val);

    QString UserName() const;
    void    UserName(QString val);

    int  UserType() const;
    void UserType(int val);

    QString UserDevice() const;
    void    UserDevice(QString val);

    QString UserGroupName() const;
    void    UserGroupName(QString val);

    void AddExtsMap(QString key, QString val);

    void                   LoadMapInfo(const QMap<QString, QString>& mapInfo);
    QMap<QString, QString> InfoMap() const;

    Json::Value ToJson();

private:
    QString m_VisitorUploadDataId;

    QString m_createTime; //时间戳
    QString m_firstType;  //第一类型 1:访客操作类型 2:客服操作类型 3:系统操作类型 ，客户端的默认为2

    QString m_secType; //第二类型
    /*
    2001:邀请访客
    2002:客服首发
    2003:创建名片
    2004:名片分配
    2005:结束对话
    2006:修改名片
    2007:访客屏蔽
    2008:对话转移
    2009:对话抢接
    2010:接管对话
    2011:介入对话
    */
    QString m_thirdType; //第三类型
    /*
    2001001:邀请访客
    2002001:客服首发
    2003001:人工/机器人创建名片
    2003002:机器人自动抓取创建名片
    2004001:名片分配
    2005001:结束对话
    2006001:修改名片
    2007001:对话中访客屏蔽
    2007002:消息记录屏蔽访客
    2007003:管理中心屏蔽访客
    2008001:对话转移
    2009001:对话抢接
    2010001:接管对话
    2011001:介入对话
    */
    QString m_companyId;       //公司id
    QString m_visitorStaticId; //访客静态id
    QString m_chatId;          //对话id
    int     m_operator;        //操作者
    /*
    1:访客
    2:客服
    3:系统
    4:机器人
    */
    QString m_exts; //扩展字段

    CommInfo m_extsMap; //扩展字段
    /* 数据格式如：
    {"A":"A1","B":"B1"}
    */
    QString m_showMethod;     //上报提示方案
    int     m_searchEngineId; //搜索引擎id
    /*
    1:google
    2:百度
    3:雅虎(yahoo)
    4:爱问(iask)
    5:有道(163)
    6:搜狗(sogou)
    7:搜搜(soso)
    8:中搜
    9:live搜索
    10:Bing
    11:盘古
    12:即刻
    13:360搜索
    14:神马搜索
    101:头条
    102:广点通
    103:快手
    105:微信小程序（学慧）
    106:微信公众号
    107:微博
    108:抖音
    109:微信小程序(原生)
    110:微信客服
    111:易维云
    112:医爱淘
    */
    QString m_visitorBrowser; //浏览器
    /*
    safari
    opera
    IE
    chrome
    baidu
    unknown
    micromessenger
    msie11
    msie
    msedge
    firefox
    android browser
    */
    QString m_visitorDeviceType; //访客设备
    /*
    app
    pc
    */

    QString m_visitorResolutionRatio; //分辨率
    QString m_visitorOs;              //操作系统
    QString m_visitorDeviceModel;     //设备型号
    QString m_visitorUserAgent;       //浏览器标头
    QString m_visitorLang;            //语言
    QString m_visitorColorDepth;      //颜色深度
    QString m_userId;                 //接待客服id
    QString m_userName;               //接待客服名称
    int     m_userType;               //客服类型
                                      /*
                                     1:人工
                                     2:机器人
                                     */

    QString m_userDevice; //客服设备 ,默认为client
    /*
    app
    client
    cloud
    */

    QString m_userGroupName; //接待分组名称
};
