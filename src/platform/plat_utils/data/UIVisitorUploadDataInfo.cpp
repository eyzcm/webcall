#include "UIVisitorUploadDataInfo.h"
#include "utils/jsonhelper.h"

UIVisitorUploadDataInfo::UIVisitorUploadDataInfo(void)
{
    this->m_firstType      = 1;
    this->m_userDevice     = "client";
    this->m_searchEngineId = 0;
}

UIVisitorUploadDataInfo::~UIVisitorUploadDataInfo(void)
{
}

UIVisitorUploadDataInfo::UIVisitorUploadDataInfo(UIVisitorUploadDataInfo* info)
{
    if (info)
    {
        VisitorUploadDataId(info->VisitorUploadDataId());

        CreateTime(info->CreateTime());

        FirstType(info->FirstType());

        SecType(info->SecType());

        ThirdType(info->ThirdType());

        CompanyId(info->CompanyId());

        VisitorStaticId(info->VisitorStaticId());

        ChatId(info->ChatId());

        Operator(info->Operator());

        Exts(info->Exts());

        ShowMethod(info->ShowMethod());

        SearchEngineId(info->SearchEngineId());

        VisitorBrowser(info->VisitorBrowser());

        VisitorDeviceType(info->VisitorDeviceType());

        VisitorResolutionRatio(info->VisitorResolutionRatio());

        VisitorOs(info->VisitorOs());

        VisitorDeviceModel(info->VisitorDeviceModel());

        VisitorUserAgent(info->VisitorUserAgent());

        VisitorLang(info->VisitorLang());

        VisitorColorDepth(info->VisitorColorDepth());

        UserId(info->UserId());

        UserName(info->UserName());

        UserType(info->UserType());

        UserDevice(info->UserDevice());

        UserGroupName(info->UserGroupName());
    }
}

QString UIVisitorUploadDataInfo::CreateTime() const
{
    return m_createTime;
}

void UIVisitorUploadDataInfo::CreateTime(QString val)
{
    m_createTime = val;
}

QString UIVisitorUploadDataInfo::FirstType() const
{
    return m_firstType;
}

void UIVisitorUploadDataInfo::FirstType(QString val)
{
    m_firstType = val;
}

QString UIVisitorUploadDataInfo::SecType() const
{
    return m_secType;
}

void UIVisitorUploadDataInfo::SecType(QString val)
{
    m_secType = val;
}

QString UIVisitorUploadDataInfo::ThirdType() const
{
    return m_thirdType;
}

void UIVisitorUploadDataInfo::ThirdType(QString val)
{
    m_thirdType = val;
}

QString UIVisitorUploadDataInfo::CompanyId() const
{
    return m_companyId;
}

void UIVisitorUploadDataInfo::CompanyId(QString val)
{
    m_companyId = val;
}

QString UIVisitorUploadDataInfo::VisitorStaticId() const
{
    return m_visitorStaticId;
}

void UIVisitorUploadDataInfo::VisitorStaticId(QString val)
{
    m_visitorStaticId = val;
}

QString UIVisitorUploadDataInfo::ChatId() const
{
    return m_chatId;
}

void UIVisitorUploadDataInfo::ChatId(QString val)
{
    m_chatId = val;
}

int UIVisitorUploadDataInfo::Operator() const
{
    return m_operator;
}

void UIVisitorUploadDataInfo::Operator(int val)
{
    m_operator = val;
}

QString UIVisitorUploadDataInfo::Exts() const
{
    return m_exts;
}

void UIVisitorUploadDataInfo::Exts(QString val)
{
    m_exts = val;
}

QString UIVisitorUploadDataInfo::ShowMethod() const
{
    return m_showMethod;
}

void UIVisitorUploadDataInfo::ShowMethod(QString val)
{
    m_showMethod = val;
}

int UIVisitorUploadDataInfo::SearchEngineId() const
{
    return m_searchEngineId;
}

void UIVisitorUploadDataInfo::SearchEngineId(int val)
{
    m_searchEngineId = val;
}

QString UIVisitorUploadDataInfo::VisitorBrowser() const
{
    return m_visitorBrowser;
}

void UIVisitorUploadDataInfo::VisitorBrowser(QString val)
{
    m_visitorBrowser = val;
}

QString UIVisitorUploadDataInfo::VisitorDeviceType() const
{
    return m_visitorDeviceType;
}

void UIVisitorUploadDataInfo::VisitorDeviceType(QString val)
{
    m_visitorDeviceType = val;
}

QString UIVisitorUploadDataInfo::VisitorResolutionRatio() const
{
    return m_visitorResolutionRatio;
}

void UIVisitorUploadDataInfo::VisitorResolutionRatio(QString val)
{
    m_visitorResolutionRatio = val;
}

QString UIVisitorUploadDataInfo::VisitorOs() const
{
    return m_visitorOs;
}

void UIVisitorUploadDataInfo::VisitorOs(QString val)
{
    m_visitorOs = val;
}

QString UIVisitorUploadDataInfo::VisitorDeviceModel() const
{
    return m_visitorDeviceModel;
}

void UIVisitorUploadDataInfo::VisitorDeviceModel(QString val)
{
    m_visitorDeviceModel = val;
}

QString UIVisitorUploadDataInfo::VisitorUserAgent() const
{
    return m_visitorUserAgent;
}

void UIVisitorUploadDataInfo::VisitorUserAgent(QString val)
{
    m_visitorUserAgent = val;
}

QString UIVisitorUploadDataInfo::VisitorLang() const
{
    return m_visitorLang;
}

void UIVisitorUploadDataInfo::VisitorLang(QString val)
{
    m_visitorLang = val;
}

QString UIVisitorUploadDataInfo::VisitorColorDepth() const
{
    return m_visitorColorDepth;
}

void UIVisitorUploadDataInfo::VisitorColorDepth(QString val)
{
    m_visitorColorDepth = val;
}

QString UIVisitorUploadDataInfo::UserId() const
{
    return m_userId;
}

void UIVisitorUploadDataInfo::UserId(QString val)
{
    m_userId = val;
}

QString UIVisitorUploadDataInfo::UserName() const
{
    return m_userName;
}

void UIVisitorUploadDataInfo::UserName(QString val)
{
    m_userName = val;
}

int UIVisitorUploadDataInfo::UserType() const
{
    return m_userType;
}

void UIVisitorUploadDataInfo::UserType(int val)
{
    m_userType = val;
}

QString UIVisitorUploadDataInfo::UserDevice() const
{
    return m_userDevice;
}

void UIVisitorUploadDataInfo::UserDevice(QString val)
{
    m_userDevice = val;
}

QString UIVisitorUploadDataInfo::UserGroupName() const
{
    return m_userGroupName;
}

void UIVisitorUploadDataInfo::UserGroupName(QString val)
{
    m_userGroupName = val;
}

void UIVisitorUploadDataInfo::LoadMapInfo(const QMap<QString, QString>& mapInfo)
{
    for (auto itor = mapInfo.begin(); itor != mapInfo.end(); itor++)
    {

        if (itor.key() == "createTime")
        {
            CreateTime(itor.value());
        }
        else if (itor.key() == "firstType")
        {
            FirstType(itor.value());
        }
        else if (itor.key() == "secType")
        {
            SecType(itor.value());
        }
        else if (itor.key() == "thirdType")
        {
            ThirdType(itor.value());
        }
        else if (itor.key() == "companyId")
        {
            CompanyId(itor.value());
        }
        else if (itor.key() == "visitorStaticId")
        {
            VisitorStaticId(itor.value());
        }
        else if (itor.key() == "chatId")
        {
            ChatId(itor.value());
        }
        else if (itor.key() == "operator")
        {
            Operator(itor.value().toInt());
        }
        else if (itor.key() == "exts")
        {
            Exts(itor.value());
        }
        else if (itor.key() == "showMethod")
        {
            ShowMethod(itor.value());
        }
        else if (itor.key() == "searchEngineId")
        {
            SearchEngineId(itor.value().toInt());
        }
        else if (itor.key() == "visitorBrowser")
        {
            VisitorBrowser(itor.value());
        }
        else if (itor.key() == "visitorDeviceType")
        {
            VisitorDeviceType(itor.value());
        }
        else if (itor.key() == "visitorResolutionRatio")
        {
            VisitorResolutionRatio(itor.value());
        }
        else if (itor.key() == "visitorOs")
        {
            VisitorOs(itor.value());
        }
        else if (itor.key() == "visitorDeviceModel")
        {
            VisitorDeviceModel(itor.value());
        }
        else if (itor.key() == "visitorUserAgent")
        {
            VisitorUserAgent(itor.value());
        }
        else if (itor.key() == "visitorLang")
        {
            VisitorLang(itor.value());
        }
        else if (itor.key() == "visitorColorDepth")
        {
            VisitorColorDepth(itor.value());
        }
        else if (itor.key() == "userId")
        {
            UserId(itor.value());
        }
        else if (itor.key() == "userName")
        {
            UserName(itor.value());
        }
        else if (itor.key() == "userType")
        {
            UserType(itor.value().toInt());
        }
        else if (itor.key() == "userDevice")
        {
            UserDevice(itor.value());
        }
        else if (itor.key() == "userGroupName")
        {
            UserGroupName(itor.value());
        }
    }
}

QMap<QString, QString> UIVisitorUploadDataInfo::InfoMap() const
{
    QMap<QString, QString> mapInfo;

    mapInfo.insert("createTime", QString("%1").arg(CreateTime()));

    mapInfo.insert("firstType", QString("%1").arg(FirstType()));

    mapInfo.insert("secType", QString("%1").arg(SecType()));

    mapInfo.insert("thirdType", QString("%1").arg(ThirdType()));

    mapInfo.insert("companyId", QString("%1").arg(CompanyId()));

    mapInfo.insert("visitorStaticId", QString("%1").arg(VisitorStaticId()));

    mapInfo.insert("chatId", QString("%1").arg(ChatId()));

    mapInfo.insert("operator", QString("%1").arg(Operator()));

    mapInfo.insert("exts", QString("%1").arg(Exts()));

    mapInfo.insert("showMethod", QString("%1").arg(ShowMethod()));

    mapInfo.insert("searchEngineId", QString("%1").arg(SearchEngineId()));

    mapInfo.insert("visitorBrowser", QString("%1").arg(VisitorBrowser()));

    mapInfo.insert("visitorDeviceType", QString("%1").arg(VisitorDeviceType()));

    mapInfo.insert("visitorResolutionRatio", QString("%1").arg(VisitorResolutionRatio()));

    mapInfo.insert("visitorOs", QString("%1").arg(VisitorOs()));

    mapInfo.insert("visitorDeviceModel", QString("%1").arg(VisitorDeviceModel()));

    mapInfo.insert("visitorUserAgent", QString("%1").arg(VisitorUserAgent()));

    mapInfo.insert("visitorLang", QString("%1").arg(VisitorLang()));

    mapInfo.insert("visitorColorDepth", QString("%1").arg(VisitorColorDepth()));

    mapInfo.insert("userId", QString("%1").arg(UserId()));

    mapInfo.insert("userName", QString("%1").arg(UserName()));

    mapInfo.insert("userType", QString("%1").arg(UserType()));

    mapInfo.insert("userDevice", QString("%1").arg(UserDevice()));

    mapInfo.insert("userGroupName", QString("%1").arg(UserGroupName()));

    return mapInfo;
}

Json::Value UIVisitorUploadDataInfo::ToJson()
{
    Json::Value            val;
    Json::Value            detail;
    QMap<QString, QString> map = InfoMap();
    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        //与服务端数据类型保持一致
        if ((iter.key() == "companyId") || (iter.key() == "firstType") || (iter.key() == "secType") || (iter.key() == "thirdType") || (iter.key() == "ChatId"))
            val[iter.key().toStdString().c_str()] = iter.value().toInt();

        else if (iter.key() == "exts")
        {
            Json::Value jsonval;
            jsonval["test"] = "";
            if (m_extsMap.GetInfoSize() > 0)
            {
                QMap<QString, QString> extmap = m_extsMap.InfoMap();
                for (auto iter = extmap.begin(); iter != extmap.end(); iter++)
                {
                    QString val                       = iter.value();
                    jsonval[iter.key().toStdString()] = iter.value().toStdString();
                }

                JsonHelper::GetJsonValueFromBody(iter.value(), jsonval);
            }
            val[iter.key().toStdString()] = jsonval;
        }
        else
        {
            val[iter.key().toStdString()] = iter.value().toStdString();
        }
    }

    return val;
}

void UIVisitorUploadDataInfo::AddExtsMap(QString key, QString val)
{
    m_extsMap.SetValueByKey(key, val);
}
